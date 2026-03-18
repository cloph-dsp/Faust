from __future__ import annotations

import argparse
import json
import math
import re
import shutil
import subprocess
import sys
from pathlib import Path

import numpy as np
import torch
import torch.nn as nn
from scipy import signal
from scipy.io import wavfile


ROOT = Path(__file__).resolve().parents[1]
DSP_FILE = ROOT / "DOD-Grunge.dsp"
ARCH_FILE = ROOT / "tools" / "faust_wav_renderer_arch.cpp"
ANALYSIS_DIR = ROOT / "analysis"
BUILD_DIR = ANALYSIS_DIR / "build"
INPUT_WAV = BUILD_DIR / "master_probe.wav"
RENDERER_CPP = BUILD_DIR / "dod_grunge_renderer.cpp"
RENDERER_EXE = BUILD_DIR / "dod_grunge_renderer.exe"
REPORT_JSON = ANALYSIS_DIR / "grunge_ab_results.json"
REPORT_MD = ANALYSIS_DIR / "grunge_ab_summary.md"

EXPLICIT_DOD_RE = re.compile(
    r"DOD FX69B Grunge Pedal \(Loud (?P<loud>\d+(?:\.\d+)?) Low (?P<low>\d+(?:\.\d+)?) High (?P<high>\d+(?:\.\d+)?) Grunge (?P<grunge>\d+(?:\.\d+)?)\)\.nam$"
)

ENHANCED_MACRO_BOUNDS: dict[str, tuple[float, float]] = {
    "grunge_push": (-0.10, 0.20),
    "grunge_curve": (-0.25, 0.25),
    "butt_push": (-0.12, 0.12),
    "face_push": (-0.12, 0.14),
    "loud_trim": (-0.15, 0.10),
    "tilt_mix": (-0.20, 0.20),
}

ENHANCED_MACRO_DEFAULT: dict[str, float] = {key: 0.0 for key in ENHANCED_MACRO_BOUNDS}


class FlatImportConv1d(nn.Conv1d):
    def load_flat(self, weights: torch.Tensor, index: int) -> int:
        if self.weight is not None:
            count = self.weight.numel()
            self.weight.data.copy_(weights[index : index + count].reshape_as(self.weight))
            index += count
        if self.bias is not None:
            count = self.bias.numel()
            self.bias.data.copy_(weights[index : index + count].reshape_as(self.bias))
            index += count
        return index


class NamWaveNetUnit(nn.Module):
    def __init__(self, condition_size: int, channels: int, kernel_size: int, dilation: int):
        super().__init__()
        self.conv = FlatImportConv1d(channels, channels, kernel_size, dilation=dilation)
        self.input_mixer = FlatImportConv1d(condition_size, channels, 1, bias=False)
        self.mix = FlatImportConv1d(channels, channels, 1)

    def load_flat(self, weights: torch.Tensor, index: int) -> int:
        index = self.conv.load_flat(weights, index)
        index = self.input_mixer.load_flat(weights, index)
        return self.mix.load_flat(weights, index)

    def forward(self, x: torch.Tensor, conditioning: torch.Tensor, out_length: int) -> tuple[torch.Tensor, torch.Tensor]:
        zconv = self.conv(x)
        z1 = zconv + self.input_mixer(conditioning)[:, :, -zconv.shape[2] :]
        post = torch.tanh(z1)
        return x[:, :, -post.shape[2] :] + self.mix(post), post[:, :, -out_length:]


class NamWaveNetStage(nn.Module):
    def __init__(self, config: dict[str, object]):
        super().__init__()
        self.kernel_size = int(config["kernel_size"])
        self.dilations = [int(value) for value in config["dilations"]]
        self.rechannel = FlatImportConv1d(int(config["input_size"]), int(config["channels"]), 1, bias=False)
        self.layers = nn.ModuleList(
            [
                NamWaveNetUnit(
                    int(config["condition_size"]),
                    int(config["channels"]),
                    self.kernel_size,
                    dilation,
                )
                for dilation in self.dilations
            ]
        )
        self.head_rechannel = FlatImportConv1d(int(config["channels"]), int(config["head_size"]), 1, bias=bool(config["head_bias"]))

    @property
    def receptive_field(self) -> int:
        return 1 + (self.kernel_size - 1) * sum(self.dilations)

    def load_flat(self, weights: torch.Tensor, index: int) -> int:
        index = self.rechannel.load_flat(weights, index)
        for layer in self.layers:
            index = layer.load_flat(weights, index)
        return self.head_rechannel.load_flat(weights, index)

    def forward(self, x: torch.Tensor, conditioning: torch.Tensor, head_input: torch.Tensor | None) -> tuple[torch.Tensor, torch.Tensor]:
        out_length = x.shape[2] - (self.receptive_field - 1)
        x = self.rechannel(x)
        for layer in self.layers:
            x, head_term = layer(x, conditioning, out_length)
            head_input = head_term if head_input is None else head_input[:, :, -out_length:] + head_term
        assert head_input is not None
        return self.head_rechannel(head_input), x


class NamWaveNetModel(nn.Module):
    def __init__(self, config: dict[str, object]):
        super().__init__()
        if config["architecture"] != "WaveNet":
            raise RuntimeError(f"Unsupported NAM architecture: {config['architecture']}")
        if config["config"]["head"] is not None:
            raise RuntimeError("Only head-less WaveNet NAM exports are supported")
        self.head_scale = float(config["config"]["head_scale"])
        self.stages = nn.ModuleList([NamWaveNetStage(stage) for stage in config["config"]["layers"]])
        self.sample_rate = int(float(config.get("sample_rate", 0)))
        flat = torch.tensor(config["weights"], dtype=torch.float32)
        index = 0
        for stage in self.stages:
            index = stage.load_flat(flat, index)
        self.eval()

    @property
    def receptive_field(self) -> int:
        return 1 + sum(stage.receptive_field - 1 for stage in self.stages)

    def forward(self, signal_in: torch.Tensor) -> torch.Tensor:
        scalar = signal_in.ndim == 1
        if scalar:
            signal_in = signal_in.unsqueeze(0)
        if signal_in.ndim == 2:
            signal_in = signal_in.unsqueeze(1)
        if signal_in.ndim != 3:
            raise RuntimeError("NAM input must be 1D, 2D, or 3D")
        pad = torch.zeros(
            (signal_in.shape[0], signal_in.shape[1], self.receptive_field - 1),
            dtype=signal_in.dtype,
            device=signal_in.device,
        )
        conditioning = torch.cat([pad, signal_in], dim=2)
        current = conditioning
        head_input = None
        for stage in self.stages:
            head_input, current = stage(current, conditioning, head_input)
        assert head_input is not None
        output = self.head_scale * head_input[:, 0, :]
        return output[0] if scalar else output


def find_executable(name: str, fallback: str | None = None) -> str:
    found = shutil.which(name)
    if found:
        return found
    if fallback and Path(fallback).exists():
        return fallback
    raise RuntimeError(f"Required executable not found: {name}")


def run_command(command: list[str], cwd: Path | None = None) -> str:
    result = subprocess.run(
        command,
        cwd=str(cwd) if cwd else None,
        capture_output=True,
        text=True,
        check=False,
    )
    if result.returncode != 0:
        raise RuntimeError(
            "Command failed:\n"
            + " ".join(command)
            + "\n\nstdout:\n"
            + result.stdout
            + "\n\nstderr:\n"
            + result.stderr
        )
    return result.stdout.strip()


def clock_to_unit(value: float) -> float:
    return float(np.clip((value - 7.0) / 10.0, 0.0, 1.0))


def parse_capture_params(path: Path) -> dict[str, float] | None:
    match = EXPLICIT_DOD_RE.match(path.name)
    if not match:
        return None
    return {
        "Grunge": clock_to_unit(float(match.group("grunge"))),
        "Butt": clock_to_unit(float(match.group("low"))),
        "Face": clock_to_unit(float(match.group("high"))),
        "Loud": clock_to_unit(float(match.group("loud"))),
        "Bypass": 0.0,
    }


def clamp(value: float, low: float, high: float) -> float:
    return float(min(max(value, low), high))


def clamp01(value: float) -> float:
    return clamp(value, 0.0, 1.0)


def sample_macro_uniform(rng: np.random.Generator) -> dict[str, float]:
    return {
        key: float(rng.uniform(low, high))
        for key, (low, high) in ENHANCED_MACRO_BOUNDS.items()
    }


def sample_macro_local(rng: np.random.Generator, center: dict[str, float], spread: float = 0.16) -> dict[str, float]:
    candidate: dict[str, float] = {}
    for key, (low, high) in ENHANCED_MACRO_BOUNDS.items():
        span = high - low
        candidate[key] = clamp(float(center[key]) + float(rng.normal(0.0, spread * span)), low, high)
    return candidate


def apply_enhanced_macro(base_params: dict[str, float], macro: dict[str, float]) -> dict[str, float]:
    g = float(base_params["Grunge"])
    b = float(base_params["Butt"])
    f = float(base_params["Face"])
    l = float(base_params["Loud"])

    tilt = float(macro["tilt_mix"]) * (2.0 * g - 1.0)
    grunge = clamp01(g + float(macro["grunge_push"]) * (1.0 - g) + float(macro["grunge_curve"]) * g * (1.0 - g))
    butt = clamp01(b + float(macro["butt_push"]) - 0.5 * tilt)
    face = clamp01(f + float(macro["face_push"]) + 0.5 * tilt)
    loud = clamp01(l + float(macro["loud_trim"]))

    return {
        "Grunge": grunge,
        "Butt": butt,
        "Face": face,
        "Loud": loud,
        "Bypass": float(base_params.get("Bypass", 0.0)),
    }


def apply_fade(signal_in: np.ndarray, sample_rate: int, fade_time: float = 0.01) -> np.ndarray:
    signal_out = np.array(signal_in, copy=True)
    fade_samples = min(int(sample_rate * fade_time), signal_out.size // 2)
    if fade_samples <= 1:
        return signal_out
    ramp = np.linspace(0.0, 1.0, fade_samples, endpoint=False, dtype=np.float32)
    signal_out[:fade_samples] *= ramp
    signal_out[-fade_samples:] *= ramp[::-1]
    return signal_out


def normalize_peak(signal_in: np.ndarray, peak: float) -> np.ndarray:
    current_peak = float(np.max(np.abs(signal_in)))
    if current_peak < 1.0e-9:
        return np.array(signal_in, copy=True)
    return signal_in * (peak / current_peak)


def build_master_probe(sample_rate: int = 48_000) -> tuple[np.ndarray, dict[str, object]]:
    rng = np.random.default_rng(0)
    sections: dict[str, tuple[int, int]] = {}
    harmonic_segments: list[dict[str, float]] = []
    dynamic_segments: list[dict[str, float]] = []
    chord_im_segments: list[dict[str, object]] = []
    chord_compress_segments: list[dict[str, object]] = []
    chunks: list[np.ndarray] = []
    cursor = 0

    def push(name: str, chunk: np.ndarray) -> tuple[int, int]:
        nonlocal cursor
        start = cursor
        cursor += chunk.size
        sections[name] = (start, cursor)
        chunks.append(chunk)
        return start, cursor

    settle = np.zeros(int(sample_rate * 0.25), dtype=np.float32)
    push("settle", settle)

    broadband_t = np.arange(int(sample_rate * 1.75), dtype=np.float32) / sample_rate
    multisine_freqs = np.array([82.41, 110.0, 146.83, 196.0, 246.94, 329.63, 440.0, 587.33, 783.99, 1046.5, 1396.91], dtype=np.float32)
    multisine = np.zeros_like(broadband_t)
    for freq, phase in zip(multisine_freqs, rng.uniform(0.0, 2.0 * np.pi, multisine_freqs.size)):
        multisine += np.sin(2.0 * np.pi * freq * broadband_t + phase, dtype=np.float32)
    multisine /= np.sqrt(multisine_freqs.size)
    noise = rng.normal(0.0, 1.0, broadband_t.size).astype(np.float32)
    band = signal.butter(4, [70.0, 6000.0], btype="bandpass", fs=sample_rate, output="sos")
    shaped_noise = signal.sosfilt(band, noise).astype(np.float32)
    shaped_noise /= float(np.std(shaped_noise) + 1.0e-9)
    modulation = 0.62 + 0.38 * np.sin(2.0 * np.pi * 0.7 * broadband_t + 0.35, dtype=np.float32)
    broadband = modulation * (0.58 * multisine + 0.42 * shaped_noise)
    broadband = normalize_peak(apply_fade(broadband.astype(np.float32), sample_rate), 0.55)
    push("broadband", broadband)

    push("bridge_a", np.zeros(int(sample_rate * 0.05), dtype=np.float32))

    harmonic_parts: list[np.ndarray] = []
    harmonic_cursor = cursor
    for freq, level in [(110.0, 0.18), (110.0, 0.45), (220.0, 0.18), (220.0, 0.45), (440.0, 0.18), (440.0, 0.45)]:
        t = np.arange(int(sample_rate * 0.28), dtype=np.float32) / sample_rate
        tone = apply_fade((level * np.sin(2.0 * np.pi * freq * t)).astype(np.float32), sample_rate, 0.015)
        start = harmonic_cursor
        harmonic_cursor += tone.size
        harmonic_segments.append({"start": float(start), "end": float(harmonic_cursor), "freq": freq, "level": level})
        harmonic_parts.append(tone)
    harmonic_block = np.concatenate(harmonic_parts) if harmonic_parts else np.zeros(0, dtype=np.float32)
    push("harmonics", harmonic_block)

    push("bridge_b", np.zeros(int(sample_rate * 0.05), dtype=np.float32))

    dynamic_parts: list[np.ndarray] = []
    dynamic_cursor = cursor
    for level in [0.02, 0.04, 0.08, 0.16, 0.32, 0.48, 0.64]:
        t = np.arange(int(sample_rate * 0.22), dtype=np.float32) / sample_rate
        segment = apply_fade((level * np.sin(2.0 * np.pi * 220.0 * t)).astype(np.float32), sample_rate, 0.01)
        start = dynamic_cursor
        dynamic_cursor += segment.size
        dynamic_segments.append({"start": float(start), "end": float(dynamic_cursor), "level": level})
        dynamic_parts.append(segment)
    dynamic_block = np.concatenate(dynamic_parts) if dynamic_parts else np.zeros(0, dtype=np.float32)
    push("dynamics", dynamic_block)

    push("bridge_c", np.zeros(int(sample_rate * 0.05), dtype=np.float32))

    chord_im_parts: list[np.ndarray] = []
    chord_im_cursor = cursor
    for chord_freqs, chord_peak in [
        ([110.0, 165.0], 0.42),
        ([220.0, 330.0], 0.42),
        ([220.0, 261.63, 330.0], 0.38),
        ([220.0, 277.18, 330.0], 0.38),
    ]:
        t = np.arange(int(sample_rate * 0.32), dtype=np.float32) / sample_rate
        chord_sig = np.zeros(t.size, dtype=np.float32)
        for f in chord_freqs:
            chord_sig += np.sin(2.0 * np.pi * float(f) * t).astype(np.float32)
        chord_sig = normalize_peak(apply_fade(chord_sig, sample_rate, 0.018), chord_peak)
        seg_start = chord_im_cursor
        chord_im_cursor += chord_sig.size
        chord_im_segments.append({"start": float(seg_start), "end": float(chord_im_cursor), "freqs": chord_freqs})
        chord_im_parts.append(chord_sig)
    push("chord_im", np.concatenate(chord_im_parts) if chord_im_parts else np.zeros(0, dtype=np.float32))

    push("bridge_d", np.zeros(int(sample_rate * 0.05), dtype=np.float32))

    chord_compress_freqs = [110.0, 165.0, 220.0]
    chord_compress_parts: list[np.ndarray] = []
    chord_compress_cursor = cursor
    for cc_level in [0.06, 0.12, 0.22, 0.36, 0.52, 0.64]:
        t = np.arange(int(sample_rate * 0.22), dtype=np.float32) / sample_rate
        chord_sig = np.zeros(t.size, dtype=np.float32)
        for f in chord_compress_freqs:
            chord_sig += np.sin(2.0 * np.pi * float(f) * t).astype(np.float32)
        chord_sig = apply_fade((cc_level * chord_sig / len(chord_compress_freqs)).astype(np.float32), sample_rate, 0.012)
        seg_start = chord_compress_cursor
        chord_compress_cursor += chord_sig.size
        chord_compress_segments.append({"start": float(seg_start), "end": float(chord_compress_cursor), "level": cc_level, "freqs": chord_compress_freqs})
        chord_compress_parts.append(chord_sig)
    push("chord_compress", np.concatenate(chord_compress_parts) if chord_compress_parts else np.zeros(0, dtype=np.float32))

    push("tail", np.zeros(int(sample_rate * 0.25), dtype=np.float32))

    master = np.concatenate(chunks).astype(np.float32)
    master = normalize_peak(master, 0.75).astype(np.float32)
    metadata = {
        "sample_rate": sample_rate,
        "sections": {name: [start, end] for name, (start, end) in sections.items()},
        "harmonic_segments": harmonic_segments,
        "dynamic_segments": dynamic_segments,
        "chord_im_segments": chord_im_segments,
        "chord_compress_segments": chord_compress_segments,
    }
    return master, metadata


def rms(signal_in: np.ndarray) -> float:
    return float(np.sqrt(np.mean(np.square(signal_in), dtype=np.float64) + 1.0e-12))


def estimate_lag(reference: np.ndarray, candidate: np.ndarray, max_lag: int = 2048) -> int:
    max_lag = min(max_lag, reference.size - 1, candidate.size - 1)
    if max_lag <= 0:
        return 0
    ref = reference.astype(np.float64) - float(np.mean(reference))
    test = candidate.astype(np.float64) - float(np.mean(candidate))
    correlation = signal.correlate(test, ref, mode="full", method="fft")
    lags = signal.correlation_lags(test.size, ref.size, mode="full")
    mask = np.abs(lags) <= max_lag
    return int(lags[mask][int(np.argmax(correlation[mask]))])


def shift_signal(signal_in: np.ndarray, lag: int, target_length: int) -> np.ndarray:
    shifted = np.zeros(target_length, dtype=np.float32)
    if lag > 0:
        usable = min(target_length, signal_in.size - lag)
        if usable > 0:
            shifted[:usable] = signal_in[lag : lag + usable]
    elif lag < 0:
        offset = -lag
        usable = min(target_length - offset, signal_in.size)
        if usable > 0:
            shifted[offset : offset + usable] = signal_in[:usable]
    else:
        usable = min(target_length, signal_in.size)
        shifted[:usable] = signal_in[:usable]
    return shifted


def best_linear_gain(reference: np.ndarray, candidate: np.ndarray) -> float:
    denominator = float(np.dot(candidate, candidate) + 1.0e-12)
    return float(np.dot(reference, candidate) / denominator)


def esr(reference: np.ndarray, candidate: np.ndarray) -> float:
    error = reference - candidate
    return float(np.sum(np.square(error), dtype=np.float64) / (np.sum(np.square(reference), dtype=np.float64) + 1.0e-12))


def log_spectral_distance(reference: np.ndarray, candidate: np.ndarray, sample_rate: int) -> float:
    _, _, ref_stft = signal.stft(reference, fs=sample_rate, nperseg=1024, noverlap=768, boundary=None)
    _, _, cand_stft = signal.stft(candidate, fs=sample_rate, nperseg=1024, noverlap=768, boundary=None)
    ref_mag = np.abs(ref_stft) + 1.0e-7
    cand_mag = np.abs(cand_stft) + 1.0e-7
    delta = 20.0 * np.log10(ref_mag) - 20.0 * np.log10(cand_mag)
    return float(np.mean(np.sqrt(np.mean(np.square(delta), axis=0))))


def extract_center(signal_in: np.ndarray, start: int, end: int, trim_fraction: float = 0.2) -> np.ndarray:
    segment = signal_in[start:end]
    trim = int(segment.size * trim_fraction * 0.5)
    if trim * 2 >= segment.size:
        return segment
    return segment[trim:-trim]


def harmonic_signature(signal_in: np.ndarray, sample_rate: int, fundamental_hz: float, harmonics: int = 8) -> np.ndarray:
    windowed = extract_center(signal_in, 0, signal_in.size, trim_fraction=0.25)
    windowed = windowed * np.hanning(windowed.size)
    spectrum = np.abs(np.fft.rfft(windowed)) + 1.0e-9
    bins = np.fft.rfftfreq(windowed.size, d=1.0 / sample_rate)
    amplitudes: list[float] = []
    for harmonic in range(1, harmonics + 1):
        target = harmonic * fundamental_hz
        if target >= sample_rate * 0.5:
            break
        center = int(np.argmin(np.abs(bins - target)))
        lo = max(0, center - 1)
        hi = min(spectrum.size, center + 2)
        amplitudes.append(float(np.max(spectrum[lo:hi])))
    harmonic_db = 20.0 * np.log10(np.asarray(amplitudes, dtype=np.float64))
    harmonic_db -= harmonic_db[0]
    return harmonic_db.astype(np.float32)


def harmonic_distance(reference: np.ndarray, candidate: np.ndarray, metadata: dict[str, object], sample_rate: int) -> float:
    distances: list[float] = []
    for item in metadata["harmonic_segments"]:
        start = int(item["start"])
        end = int(item["end"])
        freq = float(item["freq"])
        ref_sig = harmonic_signature(reference[start:end], sample_rate, freq)
        cand_sig = harmonic_signature(candidate[start:end], sample_rate, freq)
        count = min(ref_sig.size, cand_sig.size)
        if count == 0:
            continue
        distances.append(float(np.mean(np.abs(ref_sig[:count] - cand_sig[:count]))))
    return float(np.mean(distances)) if distances else 0.0


def im_product_amplitudes(signal_in: np.ndarray, sample_rate: int, fundamentals: list[float]) -> np.ndarray:
    if signal_in.size < 16:
        return np.zeros(1, dtype=np.float32)
    windowed = extract_center(signal_in, 0, signal_in.size, trim_fraction=0.20)
    windowed = windowed * np.hanning(windowed.size)
    spectrum = np.abs(np.fft.rfft(windowed.astype(np.float64))) + 1.0e-9
    bins = np.fft.rfftfreq(windowed.size, d=1.0 / sample_rate)

    def pick_bin(freq: float) -> float:
        if freq < 8.0 or freq >= sample_rate * 0.48:
            return 1.0e-9
        center = int(np.argmin(np.abs(bins - freq)))
        lo = max(0, center - 2)
        hi = min(spectrum.size, center + 3)
        return float(np.max(spectrum[lo:hi]))

    fund_mean = float(np.mean([pick_bin(f) for f in fundamentals])) + 1.0e-9
    im_freqs: list[float] = []
    n = len(fundamentals)
    for i in range(n):
        for j in range(i + 1, n):
            f1, f2 = fundamentals[i], fundamentals[j]
            im_freqs += [abs(f2 - f1), f1 + f2, abs(2.0 * f1 - f2), abs(2.0 * f2 - f1)]
    if n >= 3:
        for i in range(n):
            for j in range(i + 1, n):
                for k in range(j + 1, n):
                    f1, f2, f3 = fundamentals[i], fundamentals[j], fundamentals[k]
                    im_freqs += [abs(f1 + f2 - f3), abs(f1 - f2 + f3), abs(-f1 + f2 + f3)]
    seen: set[float] = set()
    unique: list[float] = []
    for f in im_freqs:
        key = round(f)
        if key not in seen and 8.0 <= f < sample_rate * 0.48 and all(abs(f - ff) > 4.0 for ff in fundamentals):
            seen.add(key)
            unique.append(f)
    if not unique:
        return np.zeros(1, dtype=np.float32)
    amps_db = 20.0 * np.log10(np.array([pick_bin(f) / fund_mean for f in unique], dtype=np.float64) + 1.0e-9)
    return amps_db.astype(np.float32)


def im_distance(reference: np.ndarray, candidate: np.ndarray, metadata: dict[str, object], sample_rate: int) -> float:
    distances: list[float] = []
    for item in metadata.get("chord_im_segments", []):
        start = int(item["start"])
        end = int(item["end"])
        freqs = [float(f) for f in item["freqs"]]
        ref_im = im_product_amplitudes(reference[start:end], sample_rate, freqs)
        cand_im = im_product_amplitudes(candidate[start:end], sample_rate, freqs)
        count = min(ref_im.size, cand_im.size)
        if count == 0:
            continue
        distances.append(float(np.mean(np.abs(ref_im[:count] - cand_im[:count]))))
    return float(np.mean(distances)) if distances else 0.0


def chord_compression_shape_error(reference: np.ndarray, candidate: np.ndarray, metadata: dict[str, object]) -> tuple[float, list[float], list[float]]:
    ref_curve: list[float] = []
    cand_curve: list[float] = []
    for item in metadata.get("chord_compress_segments", []):
        start = int(item["start"])
        end = int(item["end"])
        ref_curve.append(20.0 * math.log10(rms(extract_center(reference, start, end, 0.3)) + 1.0e-12))
        cand_curve.append(20.0 * math.log10(rms(extract_center(candidate, start, end, 0.3)) + 1.0e-12))
    if not ref_curve:
        return 0.0, [], []
    ref_array = np.asarray(ref_curve, dtype=np.float64)
    cand_array = np.asarray(cand_curve, dtype=np.float64)
    offset = float(np.mean(ref_array - cand_array))
    error = float(np.sqrt(np.mean(np.square(ref_array - (cand_array + offset)))))
    return error, [float(v) for v in ref_curve], [float(v) for v in cand_curve]


def compression_shape_error(reference: np.ndarray, candidate: np.ndarray, metadata: dict[str, object]) -> tuple[float, list[float], list[float]]:
    ref_curve: list[float] = []
    cand_curve: list[float] = []
    for item in metadata["dynamic_segments"]:
        start = int(item["start"])
        end = int(item["end"])
        ref_curve.append(20.0 * math.log10(rms(extract_center(reference, start, end, 0.3)) + 1.0e-12))
        cand_curve.append(20.0 * math.log10(rms(extract_center(candidate, start, end, 0.3)) + 1.0e-12))
    ref_array = np.asarray(ref_curve, dtype=np.float64)
    cand_array = np.asarray(cand_curve, dtype=np.float64)
    offset = float(np.mean(ref_array - cand_array))
    error = float(np.sqrt(np.mean(np.square(ref_array - (cand_array + offset)))))
    return error, [float(value) for value in ref_curve], [float(value) for value in cand_curve]


def composite_score(metrics: dict[str, float]) -> float:
    return float(
        metrics["level_aligned_esr"]
        + 0.5 * metrics["raw_esr"]
        + 0.05 * metrics["spectral_lsd_db"]
        + 0.02 * metrics["harmonic_distance_db"]
        + 0.10 * metrics["compression_shape_rmse_db"]
        + 0.02 * metrics.get("im_distance_db", 0.0)
        + 0.05 * metrics.get("chord_compression_shape_rmse_db", 0.0)
    )


def evaluate_pair(reference: np.ndarray, candidate: np.ndarray, metadata: dict[str, object], sample_rate: int) -> dict[str, object]:
    broadband_start, broadband_end = metadata["sections"]["broadband"]
    lag = estimate_lag(reference[broadband_start:broadband_end], candidate[broadband_start:broadband_end])
    aligned = shift_signal(candidate, lag, reference.size)

    ref_bb = reference[broadband_start:broadband_end]
    aligned_bb = aligned[broadband_start:broadband_end]
    gain = best_linear_gain(ref_bb, aligned_bb)
    gain_matched = aligned_bb * gain

    raw_gain_db = 20.0 * math.log10((rms(aligned_bb) + 1.0e-12) / (rms(ref_bb) + 1.0e-12))
    raw_esr_value = esr(ref_bb, aligned_bb)
    level_aligned_esr_value = esr(ref_bb, gain_matched)
    spectral_lsd_value = log_spectral_distance(ref_bb, gain_matched, sample_rate)
    harmonic_distance_value = harmonic_distance(reference, aligned, metadata, sample_rate)
    compression_error_value, ref_curve, cand_curve = compression_shape_error(reference, aligned, metadata)
    im_distance_value = im_distance(reference, aligned, metadata, sample_rate)
    chord_compress_error_value, chord_ref_curve, chord_cand_curve = chord_compression_shape_error(reference, aligned, metadata)

    metrics = {
        "lag_samples": int(lag),
        "raw_gain_db": float(raw_gain_db),
        "raw_esr": float(raw_esr_value),
        "level_aligned_esr": float(level_aligned_esr_value),
        "spectral_lsd_db": float(spectral_lsd_value),
        "harmonic_distance_db": float(harmonic_distance_value),
        "compression_shape_rmse_db": float(compression_error_value),
        "im_distance_db": float(im_distance_value),
        "chord_compression_shape_rmse_db": float(chord_compress_error_value),
    }
    metrics["score"] = composite_score(metrics)
    return {
        **metrics,
        "compression_ref_db": ref_curve,
        "compression_candidate_db": cand_curve,
        "chord_compression_ref_db": chord_ref_curve,
        "chord_compression_candidate_db": chord_cand_curve,
    }


def ensure_renderer(force_build: bool) -> Path:
    ANALYSIS_DIR.mkdir(exist_ok=True)
    BUILD_DIR.mkdir(parents=True, exist_ok=True)

    faust = find_executable("faust")
    gxx = find_executable("g++", r"C:\msys64\ucrt64\bin\g++.exe")
    include_dir = Path(run_command([faust, "-includedir"]))

    needs_build = force_build or not RENDERER_EXE.exists()
    if not needs_build:
        latest_source_time = max(path.stat().st_mtime for path in [DSP_FILE, ARCH_FILE])
        needs_build = latest_source_time > RENDERER_EXE.stat().st_mtime

    if needs_build:
        run_command(
            [
                faust,
                "-I",
                str(ROOT / "References" / "faust-libs"),
                "-a",
                str(ARCH_FILE),
                "-o",
                str(RENDERER_CPP),
                str(DSP_FILE),
            ]
        )
        run_command(
            [
                gxx,
                "-std=c++17",
                "-O2",
                "-I",
                str(include_dir),
                str(RENDERER_CPP),
                "-o",
                str(RENDERER_EXE),
            ]
        )
    return RENDERER_EXE


def render_faust(renderer: Path, input_wav: Path, output_wav: Path, params: dict[str, float]) -> np.ndarray:
    command = [str(renderer), str(input_wav), str(output_wav)]
    for key, value in params.items():
        command.append(f"{key}={value:.6f}")
    run_command(command)
    sample_rate, data = wavfile.read(output_wav)
    if sample_rate != 48_000:
        raise RuntimeError(f"Unexpected renderer output sample rate: {sample_rate}")
    data = data.astype(np.float32)
    if data.ndim == 2:
        data = data[:, 0]
    return data


def render_nam(model_path: Path, probe: np.ndarray) -> np.ndarray:
    with model_path.open("r", encoding="utf-8") as handle:
        config = json.load(handle)
    model = NamWaveNetModel(config).cpu().eval()
    if model.sample_rate != 48_000:
        raise RuntimeError(f"Unsupported NAM sample rate in {model_path.name}: {model.sample_rate}")
    with torch.no_grad():
        output = model(torch.from_numpy(probe.astype(np.float32))).detach().cpu().numpy().astype(np.float32)
    return output


def select_targets(limit: int | None) -> tuple[list[Path], list[str]]:
    eligible: list[Path] = []
    skipped: list[str] = []
    for path in sorted(ROOT.glob("*.nam")):
        if parse_capture_params(path) is None:
            skipped.append(path.name)
            continue
        eligible.append(path)
    if limit is not None:
        eligible = eligible[:limit]
    return eligible, skipped


def evaluate_macro_candidate(
    renderer: Path,
    probe_metadata: dict[str, object],
    nam_outputs: dict[Path, np.ndarray],
    target_info: list[tuple[int, Path, dict[str, float]]],
    macro: dict[str, float],
) -> float:
    sample_rate = int(probe_metadata["sample_rate"])
    scores: list[float] = []
    for index, model_path, base_params in target_info:
        enhanced_params = apply_enhanced_macro(base_params, macro)
        enhanced_output = render_faust(
            renderer,
            INPUT_WAV,
            BUILD_DIR / f"autotune_enhanced_{index:02d}.wav",
            {**enhanced_params, "Enhanced": 1.0},
        )
        metrics = evaluate_pair(nam_outputs[model_path], enhanced_output, probe_metadata, sample_rate)
        scores.append(float(metrics["score"]))
    return float(np.mean(scores)) if scores else math.inf


def autotune_enhanced_macro(
    renderer: Path,
    probe_metadata: dict[str, object],
    nam_outputs: dict[Path, np.ndarray],
    target_info: list[tuple[int, Path, dict[str, float]]],
    iterations: int,
    seed: int,
) -> tuple[dict[str, float], float]:
    rng = np.random.default_rng(seed)
    best_macro = dict(ENHANCED_MACRO_DEFAULT)
    best_score = evaluate_macro_candidate(renderer, probe_metadata, nam_outputs, target_info, best_macro)
    print(f"[autotune] baseline enhanced score: {best_score:.4f}")

    for iteration in range(1, iterations + 1):
        candidate = (
            sample_macro_local(rng, best_macro, spread=0.14)
            if float(rng.random()) < 0.65
            else sample_macro_uniform(rng)
        )
        score = evaluate_macro_candidate(renderer, probe_metadata, nam_outputs, target_info, candidate)
        if score < best_score:
            best_score = score
            best_macro = candidate
            print(f"[autotune] iter {iteration}/{iterations}: improved -> {best_score:.4f}")

    return best_macro, best_score


def write_summary(results: dict[str, object]) -> None:
    lines = [
        "# DOD Grunge A/B Summary",
        "",
        "Composite score = level_aligned_esr + 0.5 * raw_esr + 0.05 * spectral_lsd_db + 0.02 * harmonic_distance_db + 0.10 * compression_shape_rmse_db + 0.02 * im_distance_db + 0.05 * chord_compression_shape_rmse_db.",
        "Lower is better.",
        "",
        f"Eligible targets: {results['aggregate']['target_count']}",
        f"Stock wins: {results['aggregate']['stock_better_count']}",
        f"Enhanced wins: {results['aggregate']['enhanced_better_count']}",
        "",
        "| Target | Stock Score | Enhanced Score | Better |",
        "| --- | ---: | ---: | --- |",
    ]
    for target in results["targets"]:
        better = "Stock" if target["stock"]["score"] <= target["enhanced"]["score"] else "Enhanced"
        lines.append(
            f"| {target['target']} | {target['stock']['score']:.4f} | {target['enhanced']['score']:.4f} | {better} |"
        )
    lines.extend(
        [
            "",
            "## Aggregate",
            "",
            f"Stock mean score: {results['aggregate']['stock_mean_score']:.4f}",
            f"Enhanced mean score: {results['aggregate']['enhanced_mean_score']:.4f}",
            f"Stock mean aligned ESR: {results['aggregate']['stock_mean_aligned_esr']:.4f}",
            f"Enhanced mean aligned ESR: {results['aggregate']['enhanced_mean_aligned_esr']:.4f}",
            "",
            "## Enhanced Macro",
            "",
        ]
    )
    for key, value in results["aggregate"]["enhanced_macro"].items():
        lines.append(f"- {key}: {value:+.4f}")
    REPORT_MD.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(description="Compare the Faust DOD Grunge DSP against explicit-parameter stock DOD NAM captures.")
    parser.add_argument("--limit", type=int, default=None, help="Only evaluate the first N eligible NAM captures.")
    parser.add_argument("--force-build", action="store_true", help="Force regeneration and recompilation of the Faust renderer.")
    parser.add_argument("--autotune-enhanced", action="store_true", help="Run a constrained search for an improved Enhanced macro profile before the full evaluation.")
    parser.add_argument("--autotune-iters", type=int, default=48, help="Number of random/local-search iterations for enhanced auto-tuning.")
    parser.add_argument("--autotune-targets", type=int, default=3, help="How many eligible targets to use for the enhanced auto-tune stage.")
    parser.add_argument("--autotune-seed", type=int, default=7, help="Random seed for enhanced auto-tuning.")
    args = parser.parse_args()

    torch.set_num_threads(1)
    renderer = ensure_renderer(args.force_build)

    probe, metadata = build_master_probe()
    wavfile.write(INPUT_WAV, int(metadata["sample_rate"]), probe)

    targets, skipped = select_targets(args.limit)
    if not targets:
        raise RuntimeError("No explicit-parameter DOD NAM captures were found.")

    target_info: list[tuple[int, Path, dict[str, float]]] = []
    for index, model_path in enumerate(targets, start=1):
        base_params = parse_capture_params(model_path)
        assert base_params is not None
        target_info.append((index, model_path, base_params))

    nam_cache: dict[Path, np.ndarray] = {}

    enhanced_macro = dict(ENHANCED_MACRO_DEFAULT)
    if args.autotune_enhanced:
        tune_count = max(1, min(int(args.autotune_targets), len(target_info)))
        tune_targets = target_info[:tune_count]
        print(f"[autotune] loading NAM outputs for {tune_count} target(s)...")
        for index, model_path, _ in tune_targets:
            print(f"[autotune NAM {index}/{tune_count}] {model_path.name}")
            nam_cache[model_path] = render_nam(model_path, probe)
        enhanced_macro, tuned_score = autotune_enhanced_macro(
            renderer,
            metadata,
            nam_cache,
            tune_targets,
            max(1, int(args.autotune_iters)),
            int(args.autotune_seed),
        )
        print(f"[autotune] best enhanced score: {tuned_score:.4f}")
        print("[autotune] best macro:")
        for key in sorted(enhanced_macro):
            print(f"  {key}={enhanced_macro[key]:+.6f}")

    results: list[dict[str, object]] = []
    stock_scores: list[float] = []
    enhanced_scores: list[float] = []
    stock_aligned_esr: list[float] = []
    enhanced_aligned_esr: list[float] = []
    stock_better_count = 0
    enhanced_better_count = 0

    for index, model_path, base_params in target_info:
        print(f"[{index}/{len(targets)}] {model_path.name}")
        if model_path not in nam_cache:
            nam_cache[model_path] = render_nam(model_path, probe)
        nam_output = nam_cache[model_path]

        enhanced_params = apply_enhanced_macro(base_params, enhanced_macro)
        stock_output = render_faust(renderer, INPUT_WAV, BUILD_DIR / f"stock_{index:02d}.wav", {**base_params, "Enhanced": 0.0})
        enhanced_output = render_faust(renderer, INPUT_WAV, BUILD_DIR / f"enhanced_{index:02d}.wav", {**enhanced_params, "Enhanced": 1.0})

        stock_metrics = evaluate_pair(nam_output, stock_output, metadata, int(metadata["sample_rate"]))
        enhanced_metrics = evaluate_pair(nam_output, enhanced_output, metadata, int(metadata["sample_rate"]))

        stock_scores.append(float(stock_metrics["score"]))
        enhanced_scores.append(float(enhanced_metrics["score"]))
        stock_aligned_esr.append(float(stock_metrics["level_aligned_esr"]))
        enhanced_aligned_esr.append(float(enhanced_metrics["level_aligned_esr"]))

        if stock_metrics["score"] <= enhanced_metrics["score"]:
            stock_better_count += 1
        else:
            enhanced_better_count += 1

        results.append(
            {
                "target": model_path.name,
                "params": base_params,
                "enhanced_params": enhanced_params,
                "stock": stock_metrics,
                "enhanced": enhanced_metrics,
            }
        )

    report = {
        "sample_rate": int(metadata["sample_rate"]),
        "probe_length_samples": int(probe.size),
        "skipped_targets": skipped,
        "targets": results,
        "aggregate": {
            "target_count": len(results),
            "stock_better_count": stock_better_count,
            "enhanced_better_count": enhanced_better_count,
            "stock_mean_score": float(np.mean(stock_scores)),
            "enhanced_mean_score": float(np.mean(enhanced_scores)),
            "stock_mean_aligned_esr": float(np.mean(stock_aligned_esr)),
            "enhanced_mean_aligned_esr": float(np.mean(enhanced_aligned_esr)),
            "enhanced_macro": enhanced_macro,
        },
    }

    REPORT_JSON.write_text(json.dumps(report, indent=2), encoding="utf-8")
    write_summary(report)

    print(f"Wrote {REPORT_JSON}")
    print(f"Wrote {REPORT_MD}")
    return 0


if __name__ == "__main__":
    sys.exit(main())