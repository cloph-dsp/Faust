#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "faust/dsp/dsp.h"
#include "faust/gui/MapUI.h"
#include "faust/gui/meta.h"

<<includeIntrinsic>>

<<includeclass>>

namespace {

struct WavData {
    int sample_rate = 0;
    int num_channels = 0;
    std::vector<float> interleaved;
};

std::uint16_t read_u16_le(std::istream& stream)
{
    std::uint8_t bytes[2];
    stream.read(reinterpret_cast<char*>(bytes), sizeof(bytes));
    if (!stream) {
        throw std::runtime_error("Unexpected end of file while reading 16-bit value");
    }
    return std::uint16_t(bytes[0]) | (std::uint16_t(bytes[1]) << 8);
}

std::uint32_t read_u32_le(std::istream& stream)
{
    std::uint8_t bytes[4];
    stream.read(reinterpret_cast<char*>(bytes), sizeof(bytes));
    if (!stream) {
        throw std::runtime_error("Unexpected end of file while reading 32-bit value");
    }
    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) | (std::uint32_t(bytes[3]) << 24);
}

void write_u16_le(std::ostream& stream, std::uint16_t value)
{
    const std::uint8_t bytes[2] = {
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
    };
    stream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void write_u32_le(std::ostream& stream, std::uint32_t value)
{
    const std::uint8_t bytes[4] = {
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>((value >> 8) & 0xFF),
        static_cast<std::uint8_t>((value >> 16) & 0xFF),
        static_cast<std::uint8_t>((value >> 24) & 0xFF),
    };
    stream.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
}

void skip_chunk(std::istream& stream, std::uint32_t size)
{
    stream.seekg(static_cast<std::streamoff>(size + (size & 1U)), std::ios::cur);
    if (!stream) {
        throw std::runtime_error("Failed to skip WAV chunk");
    }
}

WavData read_wav_file(const std::string& path)
{
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("Failed to open input WAV: " + path);
    }

    char riff[4];
    char wave[4];
    stream.read(riff, sizeof(riff));
    const std::uint32_t riff_size = read_u32_le(stream);
    (void)riff_size;
    stream.read(wave, sizeof(wave));
    if (std::memcmp(riff, "RIFF", 4) != 0 || std::memcmp(wave, "WAVE", 4) != 0) {
        throw std::runtime_error("Unsupported WAV container in: " + path);
    }

    bool has_format = false;
    std::uint16_t audio_format = 0;
    std::uint16_t num_channels = 0;
    std::uint32_t sample_rate = 0;
    std::uint16_t bits_per_sample = 0;
    std::vector<std::uint8_t> data_bytes;

    while (stream && (!has_format || data_bytes.empty())) {
        char chunk_id[4];
        stream.read(chunk_id, sizeof(chunk_id));
        if (!stream) {
            break;
        }
        const std::uint32_t chunk_size = read_u32_le(stream);

        if (std::memcmp(chunk_id, "fmt ", 4) == 0) {
            audio_format = read_u16_le(stream);
            num_channels = read_u16_le(stream);
            sample_rate = read_u32_le(stream);
            const std::uint32_t byte_rate = read_u32_le(stream);
            const std::uint16_t block_align = read_u16_le(stream);
            bits_per_sample = read_u16_le(stream);
            (void)byte_rate;
            (void)block_align;
            if (chunk_size > 16) {
                skip_chunk(stream, chunk_size - 16);
            }
            has_format = true;
        } else if (std::memcmp(chunk_id, "data", 4) == 0) {
            data_bytes.resize(chunk_size);
            stream.read(reinterpret_cast<char*>(data_bytes.data()), static_cast<std::streamsize>(chunk_size));
            if (!stream) {
                throw std::runtime_error("Failed to read WAV data chunk: " + path);
            }
            if (chunk_size & 1U) {
                stream.get();
            }
        } else {
            skip_chunk(stream, chunk_size);
        }
    }

    if (!has_format || data_bytes.empty()) {
        throw std::runtime_error("Incomplete WAV file: " + path);
    }
    if (num_channels == 0 || sample_rate == 0) {
        throw std::runtime_error("Invalid WAV format in: " + path);
    }

    WavData wav;
    wav.sample_rate = static_cast<int>(sample_rate);
    wav.num_channels = static_cast<int>(num_channels);

    if (audio_format == 3 && bits_per_sample == 32) {
        const std::size_t sample_count = data_bytes.size() / 4;
        wav.interleaved.resize(sample_count);
        for (std::size_t index = 0; index < sample_count; ++index) {
            const std::uint32_t word = std::uint32_t(data_bytes[4 * index]) |
                                       (std::uint32_t(data_bytes[4 * index + 1]) << 8) |
                                       (std::uint32_t(data_bytes[4 * index + 2]) << 16) |
                                       (std::uint32_t(data_bytes[4 * index + 3]) << 24);
            float value = 0.0f;
            std::memcpy(&value, &word, sizeof(float));
            wav.interleaved[index] = value;
        }
    } else if (audio_format == 1 && bits_per_sample == 16) {
        const std::size_t sample_count = data_bytes.size() / 2;
        wav.interleaved.resize(sample_count);
        for (std::size_t index = 0; index < sample_count; ++index) {
            const std::uint16_t word = std::uint16_t(data_bytes[2 * index]) |
                                       (std::uint16_t(data_bytes[2 * index + 1]) << 8);
            const std::int16_t sample = static_cast<std::int16_t>(word);
            wav.interleaved[index] = static_cast<float>(sample) / 32768.0f;
        }
    } else {
        throw std::runtime_error("Only 16-bit PCM and 32-bit float WAV files are supported");
    }

    return wav;
}

void write_wav_file(const std::string& path, int sample_rate, int num_channels, const std::vector<float>& interleaved)
{
    std::ofstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("Failed to open output WAV: " + path);
    }

    const std::uint32_t data_size = static_cast<std::uint32_t>(interleaved.size() * sizeof(float));
    const std::uint32_t riff_size = 4 + (8 + 16) + (8 + data_size);

    stream.write("RIFF", 4);
    write_u32_le(stream, riff_size);
    stream.write("WAVE", 4);

    stream.write("fmt ", 4);
    write_u32_le(stream, 16);
    write_u16_le(stream, 3);
    write_u16_le(stream, static_cast<std::uint16_t>(num_channels));
    write_u32_le(stream, static_cast<std::uint32_t>(sample_rate));
    write_u32_le(stream, static_cast<std::uint32_t>(sample_rate * num_channels * sizeof(float)));
    write_u16_le(stream, static_cast<std::uint16_t>(num_channels * sizeof(float)));
    write_u16_le(stream, 32);

    stream.write("data", 4);
    write_u32_le(stream, data_size);
    stream.write(reinterpret_cast<const char*>(interleaved.data()), static_cast<std::streamsize>(data_size));

    if (!stream) {
        throw std::runtime_error("Failed to write output WAV: " + path);
    }
}

bool has_param(MapUI& ui, const std::string& name)
{
    return ui.getFullpathMap().count(name) > 0 || ui.getShortnameMap().count(name) > 0 || ui.getLabelMap().count(name) > 0;
}

void print_usage(const char* executable)
{
    std::cerr << "Usage: " << executable << " <input.wav> <output.wav> [param=value ...] [--block-size=N]\n";
    std::cerr << "       " << executable << " --list-params\n";
}

void print_params(MapUI& ui)
{
    for (int index = 0; index < ui.getParamsCount(); ++index) {
        std::cout << ui.getParamShortname(index) << "\t" << ui.getParamAddress(index) << "\n";
    }
}

void apply_args(MapUI& ui, int argc, char* argv[], int start_index, int& block_size)
{
    for (int index = start_index; index < argc; ++index) {
        const std::string argument(argv[index]);
        if (argument.rfind("--block-size=", 0) == 0) {
            block_size = std::max(1, std::stoi(argument.substr(13)));
            continue;
        }
        if (argument == "--block-size") {
            if (index + 1 >= argc) {
                throw std::runtime_error("Missing value after --block-size");
            }
            block_size = std::max(1, std::stoi(argv[++index]));
            continue;
        }

        const std::size_t split = argument.find('=');
        if (split == std::string::npos) {
            throw std::runtime_error("Expected param=value argument, got: " + argument);
        }

        const std::string name = argument.substr(0, split);
        const float value = std::stof(argument.substr(split + 1));
        if (!has_param(ui, name)) {
            throw std::runtime_error("Unknown Faust parameter: " + name);
        }
        ui.setParamValue(name, value);
    }
}

std::vector<std::vector<FAUSTFLOAT>> prepare_inputs(const WavData& wav, int dsp_inputs)
{
    const std::size_t frame_count = wav.interleaved.size() / static_cast<std::size_t>(wav.num_channels);
    std::vector<std::vector<FAUSTFLOAT>> channels(std::max(dsp_inputs, 1), std::vector<FAUSTFLOAT>(frame_count, 0.0f));

    for (std::size_t frame = 0; frame < frame_count; ++frame) {
        for (int channel = 0; channel < std::max(dsp_inputs, 1); ++channel) {
            const int source_channel = std::min(channel, wav.num_channels - 1);
            channels[channel][frame] = wav.interleaved[frame * static_cast<std::size_t>(wav.num_channels) + static_cast<std::size_t>(source_channel)];
        }
    }

    return channels;
}

std::vector<float> interleave_outputs(const std::vector<std::vector<FAUSTFLOAT>>& outputs)
{
    const std::size_t frame_count = outputs.empty() ? 0 : outputs.front().size();
    const int num_channels = static_cast<int>(outputs.size());
    std::vector<float> interleaved(frame_count * static_cast<std::size_t>(num_channels), 0.0f);

    for (std::size_t frame = 0; frame < frame_count; ++frame) {
        for (int channel = 0; channel < num_channels; ++channel) {
            interleaved[frame * static_cast<std::size_t>(num_channels) + static_cast<std::size_t>(channel)] = outputs[channel][frame];
        }
    }

    return interleaved;
}

}  // namespace

int main(int argc, char* argv[])
{
    try {
        mydsp dsp;
        MapUI ui;
        dsp.buildUserInterface(&ui);

        if (argc == 2 && std::string(argv[1]) == "--list-params") {
            print_params(ui);
            return 0;
        }
        if (argc < 3) {
            print_usage(argv[0]);
            return 1;
        }

        const std::string input_path(argv[1]);
        const std::string output_path(argv[2]);
        int block_size = 256;

        const WavData wav = read_wav_file(input_path);
        if (wav.interleaved.empty()) {
            throw std::runtime_error("Input WAV is empty: " + input_path);
        }

        dsp.init(wav.sample_rate);
        apply_args(ui, argc, argv, 3, block_size);

        const std::size_t frame_count = wav.interleaved.size() / static_cast<std::size_t>(wav.num_channels);
        const int num_inputs = dsp.getNumInputs();
        const int num_outputs = dsp.getNumOutputs();

        std::vector<std::vector<FAUSTFLOAT>> input_channels = prepare_inputs(wav, num_inputs);
        std::vector<std::vector<FAUSTFLOAT>> output_channels(std::max(num_outputs, 1), std::vector<FAUSTFLOAT>(frame_count, 0.0f));

        std::vector<FAUSTFLOAT*> input_ptrs(std::max(num_inputs, 1), nullptr);
        std::vector<FAUSTFLOAT*> output_ptrs(std::max(num_outputs, 1), nullptr);

        for (std::size_t offset = 0; offset < frame_count; offset += static_cast<std::size_t>(block_size)) {
            const int frames = static_cast<int>(std::min<std::size_t>(static_cast<std::size_t>(block_size), frame_count - offset));
            for (int channel = 0; channel < num_inputs; ++channel) {
                input_ptrs[channel] = input_channels[channel].data() + offset;
            }
            for (int channel = 0; channel < num_outputs; ++channel) {
                output_ptrs[channel] = output_channels[channel].data() + offset;
            }
            dsp.compute(frames, (num_inputs > 0) ? input_ptrs.data() : nullptr, output_ptrs.data());
        }

        write_wav_file(output_path, wav.sample_rate, num_outputs, interleave_outputs(output_channels));
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "ERROR: " << error.what() << "\n";
        return 1;
    }
}