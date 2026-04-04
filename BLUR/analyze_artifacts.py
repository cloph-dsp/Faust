#!/usr/bin/env python3
"""Analyze artifact test WAV file for zipper noise, clipping, and signal integrity."""

import wave
import numpy as np
import sys

def analyze_wav(filename):
    """Analyze WAV file for artifacts and signal quality."""
    
    try:
        with wave.open(filename, 'rb') as wav:
            n_channels = wav.getnchannels()
            sample_width = wav.getsampwidth()
            framerate = wav.getframerate()
            n_frames = wav.getnframes()
            
            duration = n_frames / framerate
            
            # Read audio
            frames = wav.readframes(n_frames)
            audio = np.frombuffer(frames, dtype=np.int16).reshape(-1, n_channels)
            audio = audio / 32768.0  # Normalize to [-1, 1]
            
            print('=' * 70)
            print('SpectralBlur Artifact Test Analysis')
            print('=' * 70)
            print()
            print('File: spectralblur_artifact_test.wav')
            print(f'Channels:        {n_channels}')
            print(f'Sample rate:     {framerate} Hz')
            print(f'Duration:        {duration:.2f} seconds')
            print(f'Total samples:   {n_frames:,}')
            print()
            
            # Peak and RMS analysis per channel
            print('Signal Amplitude Analysis:')
            print('-' * 70)
            for c in range(n_channels):
                signal = audio[:, c]
                peak = np.max(np.abs(signal))
                rms = np.sqrt(np.mean(signal**2))
                peak_db = 20 * np.log10(peak) if peak > 0 else -np.inf
                rms_db = 20 * np.log10(rms) if rms > 0 else -np.inf
                print(f'Channel {c+1}:')
                print(f'  Peak level:      {peak:.4f} ({peak_db:+.1f} dB FS)')
                print(f'  RMS level:       {rms:.4f} ({rms_db:+.1f} dB FS)')
            
            print()
            print('Signal Integrity Checks:')
            print('-' * 70)
            
            # Check for clipping
            clipped = np.any(np.abs(audio) > 0.99)
            print(f'No clipping:     {"PASS" if not clipped else "FAIL (signal clipped)"}')
            
            # Check for silence
            silent = np.max(np.abs(audio)) < 0.01
            print(f'Not silent:      {"PASS" if not silent else "FAIL (no signal)"}')
            
            # Check for NaN/Inf
            valid = np.all(np.isfinite(audio))
            print(f'No NaN/Inf:      {"PASS" if valid else "FAIL (invalid samples)"}')
            
            print()
            print('Artifact Indicators (Zipper Noise Detection):')
            print('-' * 70)
            
            # Check for discontinuous jumps (zipper noise indicator)
            combined = audio.mean(axis=1)  # Mix to mono for analysis
            diffs = np.abs(np.diff(combined))
            max_jump = np.max(diffs)
            mean_jump = np.mean(diffs)
            std_jump = np.std(diffs)
            
            print(f'Max sample jump:  {max_jump:.8f}')
            print(f'Mean jump:        {mean_jump:.8f}')
            print(f'Std dev:          {std_jump:.8f}')
            
            # Zipper noise threshold (aggressive automation can exceed 0.01)
            if max_jump > 0.5:
                severity = 'FAIL'
                detail = 'Severe discontinuity (likely distortion or crash)'
            elif max_jump > 0.1:
                severity = 'WARN'
                detail = 'Large jumps (possible zipper noise)'
            elif max_jump > 0.05:
                severity = 'WARN'
                detail = 'Moderate jumps (check spectrogram)'
            else:
                severity = 'PASS'
                detail = 'Smooth transitions (no obvious zippers)'
            
            print(f'Zipper risk:     {severity}: {detail}')
            
            print()
            print('Spectral Analysis (Time-Domain Energy):')
            print('-' * 70)
            
            # Divide into sweep regions
            warmup_end = int(5 * framerate)
            sweep_start = int(5 * framerate)
            sweep_end = int(65 * framerate)
            tail_start = int(65 * framerate)
            
            warmup = combined[:warmup_end]
            sweep = combined[sweep_start:sweep_end]
            tail = combined[tail_start:]
            
            print(f'Warmup (0-5s):   RMS = {np.sqrt(np.mean(warmup**2)):.4f}')
            print(f'Sweep (5-65s):   RMS = {np.sqrt(np.mean(sweep**2)):.4f}')
            print(f'Tail (65-70s):   RMS = {np.sqrt(np.mean(tail**2)):.4f}')
            
            # Energy stability during sweep
            sweep_chunks = np.array_split(sweep, 10)
            chunk_rms = [np.sqrt(np.mean(chunk**2)) for chunk in sweep_chunks]
            energy_variance = np.std(chunk_rms) / np.mean(chunk_rms)
            
            print(f'Energy stability: {energy_variance:.2%} variation during sweep')
            if energy_variance > 0.3:
                print('                 ⚠ Note: Significant energy changes (expected with blur sweep)')
            
            print()
            print('Test Result:')
            print('=' * 70)
            
            # Final verdict
            all_pass = (not clipped) and (not silent) and valid and (max_jump < 0.1)
            
            if all_pass:
                print('✓ ARTIFACT TEST: PASS')
                print('  No critical artifacts detected. Audio is clean and valid.')
                print('  → Safe for release (pending DAW integration test)')
            elif max_jump < 0.05:
                print('✓ ARTIFACT TEST: PASS (with minor quirks)')
                print('  Smooth automation detected. Minimal artifact risk.')
                print('  → Safe for release')
            else:
                print('⚠ ARTIFACT TEST: CHECK MANUALLY')
                print('  Mathematical analysis shows issues. Listen to WAV in:')
                print('  - Sonic Visualiser (spectrogram view)')
                print('  - Your DAW (automation view)')
                print('  Decision: Continue or revert to faust-optimize')
            
            print()
            print('Next Steps:')
            print('  1. Listen to spectralblur_artifact_test.wav in audio player')
            print('  2. Open in Sonic Visualiser for spectrogram inspection')
            print('  3. Look for smooth blur sweep (5-65s) with no clicks/zippers')
            print('  4. If clean: → Proceed to DAW integration test')
            print('  5. If artifacts: → Return to faust-optimize for smoothing tuning')
            print()
            
    except Exception as e:
        print(f'Error analyzing WAV: {e}')
        return False
    
    return True

if __name__ == '__main__':
    analyze_wav('spectralblur_artifact_test.wav')
