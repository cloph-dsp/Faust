#include "spectralblur.h"
#include <cmath>
#include <fstream>
#include <random>
#include <vector>
#include <cstring>

// ─────────────────────────────────────────────────────────────────────────────
// Simple WAV file writer (RIFF format, 16-bit PCM)
// ─────────────────────────────────────────────────────────────────────────────
class WavWriter {
private:
    std::ofstream file;
    int numChannels;
    int sampleRate;
    int numSamples;
    long dataChunkPos;

    struct WAVHeader {
        char riff[4] = {'R', 'I', 'F', 'F'};
        int32_t fileSize;
        char wave[4] = {'W', 'A', 'V', 'E'};
        char fmt[4] = {'f', 'm', 't', ' '};
        int32_t fmtSize = 16;
        int16_t audioFormat = 1;  // PCM
        int16_t numCh;
        int32_t sampleRateVal;
        int32_t byteRate;
        int16_t blockAlign;
        int16_t bitsPerSample = 16;
        char data[4] = {'d', 'a', 't', 'a'};
        int32_t dataSize;
    };

public:
    WavWriter(const char* filename, int channels, int sr)
        : numChannels(channels), sampleRate(sr), numSamples(0), dataChunkPos(0)
    {
        file.open(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open WAV file for writing");
        }
        // Write header placeholder (will update later)
        WAVHeader hdr;
        hdr.numCh = static_cast<int16_t>(channels);
        hdr.sampleRateVal = static_cast<int32_t>(sr);
        hdr.byteRate = static_cast<int32_t>(sr * channels * 2);
        hdr.blockAlign = static_cast<int16_t>(channels * 2);
        file.write(reinterpret_cast<char*>(&hdr), sizeof(WAVHeader));
        dataChunkPos = file.tellp();
    }

    void writeSamples(const float* const* channels, int numSamples_)
    {
        for (int i = 0; i < numSamples_; i++) {
            for (int c = 0; c < numChannels; c++) {
                float sample = channels[c][i];
                // Clamp to [-1, 1]
                sample = std::max(-1.0f, std::min(1.0f, sample));
                // Convert to 16-bit PCM
                int16_t pcm = static_cast<int16_t>(sample * 32767.0f);
                file.write(reinterpret_cast<char*>(&pcm), sizeof(int16_t));
            }
        }
        numSamples += numSamples_;
    }

    void close()
    {
        if (!file.is_open()) return;

        int32_t dataSize = numSamples * numChannels * 2;
        int32_t fileSize = 36 + dataSize;

        // Update header
        file.seekp(4);
        file.write(reinterpret_cast<char*>(&fileSize), sizeof(int32_t));

        file.seekp(24);
        int32_t sr = sampleRate;
        file.write(reinterpret_cast<char*>(&sr), sizeof(int32_t));

        int32_t byteRate = sampleRate * numChannels * 2;
        file.write(reinterpret_cast<char*>(&byteRate), sizeof(int32_t));

        int16_t blockAlign = numChannels * 2;
        file.write(reinterpret_cast<char*>(&blockAlign), sizeof(int16_t));

        file.seekp(40);
        file.write(reinterpret_cast<char*>(&dataSize), sizeof(int32_t));

        file.close();
    }

    ~WavWriter() { close(); }
};

// ─────────────────────────────────────────────────────────────────────────────
// Pink Noise Generator (1/f spectrum)
// ─────────────────────────────────────────────────────────────────────────────
class PinkNoiseGenerator {
private:
    std::mt19937 gen{42};
    std::uniform_real_distribution<float> dis{-1.0f, 1.0f};
    float b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0, b5 = 0, b6 = 0;

public:
    float nextSample()
    {
        float white = dis(gen);
        b0 = 0.049922035f * white + 0.950177995f * b0;
        b1 = 0.049922035f * b0 + 0.950177995f * b1;
        b2 = 0.049922035f * b1 + 0.950177995f * b2;
        b3 = 0.049922035f * b2 + 0.950177995f * b3;
        b4 = 0.049922035f * b3 + 0.950177995f * b4;
        b5 = 0.049922035f * b4 + 0.950177995f * b5;
        b6 = 0.049922035f * b5 + 0.950177995f * b6;
        return b6 * 0.11f;  // Scale down to prevent clipping
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Main artifact test: Automation sweep (Blur 0→100% @ 1Hz over 60 sec)
// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    using namespace spectralblur;

    const int sampleRate = 44100;
    const int blockSize = 512;
    const int numChannels = 2;  // Stereo
    const float durationSeconds = 70.0f;  // 70 seconds to ensure full sweep + tail
    const int totalSamples = static_cast<int>(sampleRate * durationSeconds);

    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  SpectralBlur Artifact Test Harness                               ║\n");
    printf("║  Automation Sweep: Blur 0→100% @ 1Hz over 60 seconds              ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");

    printf("Configuration:\n");
    printf("  Sample rate:     %d Hz\n", sampleRate);
    printf("  Block size:      %d samples\n", blockSize);
    printf("  Channels:        %d (stereo)\n", numChannels);
    printf("  Duration:        %.1f seconds\n", durationSeconds);
    printf("  Test signal:     Pink noise (1/f spectrum)\n");
    printf("  Sweep target:    Blur 0→100%% linearly\n");
    printf("  Sweep period:    ~60 seconds (1 Hz rate)\n\n");

    // Initialize DSP
    Processor processor;
    processor.prepare(sampleRate, blockSize, numChannels);

    Parameters params;
    params.blurAmountPercent = 50.0f;
    params.varianceType = static_cast<int>(VarianceType::Fixed);  // No modulation for clean test
    params.randomizePhases = false;  // Disable phase randomization to isolate blur artifacts
    params.fftSizeIndex = 5;  // 16384 FFT (good frequency resolution)
    processor.setParameters(params);

    // Latency from DSP
    int latencySamples = processor.getLatencySamples();
    printf("DSP Latency:     %d samples (%.2f ms @ %d Hz)\n\n", 
           latencySamples, 1000.0f * latencySamples / sampleRate, sampleRate);

    // Pink noise generator
    PinkNoiseGenerator noiseGen;

    // Output file
    WavWriter wavOut("spectralblur_artifact_test.wav", numChannels, sampleRate);

    printf("Rendering... ");
    fflush(stdout);

    // Process in blocks
    std::vector<float> inputBlock(blockSize);
    std::vector<float> outputBlock(blockSize);
    std::vector<float*> inputPtrs(numChannels), outputPtrs(numChannels);

    for (int c = 0; c < numChannels; c++) {
        inputPtrs[c] = inputBlock.data();
        outputPtrs[c] = outputBlock.data();
    }

    float progressUpdate = 0.0f;
    for (int sampleIdx = 0; sampleIdx < totalSamples; sampleIdx += blockSize) {
        int currentBlockSize = std::min(blockSize, totalSamples - sampleIdx);

        // Generate pink noise input
        for (int i = 0; i < currentBlockSize; i++) {
            inputBlock[i] = noiseGen.nextSample();
        }

        // Automate blur parameter linearly over 60 seconds (after 5 sec warmup)
        float blurSweepStart = 5.0f;  // Start automation at 5 seconds (after DSP settles)
        float blurSweepEnd = 65.0f;   // End at 65 seconds
        float timeInSeconds = static_cast<float>(sampleIdx) / sampleRate;

        if (timeInSeconds >= blurSweepStart && timeInSeconds < blurSweepEnd) {
            // Linear sweep: 0→100% over sweep duration
            float sweepPhase = (timeInSeconds - blurSweepStart) / (blurSweepEnd - blurSweepStart);
            params.blurAmountPercent = sweepPhase * 100.0f;
            processor.setParameters(params);
        } else if (timeInSeconds < blurSweepStart) {
            // Warmup: keep blur at 50%
            params.blurAmountPercent = 50.0f;
            processor.setParameters(params);
        } else {
            // Tail: hold at 100% for tail capture
            params.blurAmountPercent = 100.0f;
            processor.setParameters(params);
        }

        // Process stereo (both channels get same input for consistency)
        processor.processBlock(inputPtrs.data(), outputPtrs.data(), numChannels, currentBlockSize);

        // Write output
        wavOut.writeSamples(outputPtrs.data(), currentBlockSize);

        // Progress indicator
        float progress = static_cast<float>(sampleIdx) / totalSamples;
        if (progress - progressUpdate >= 0.1f) {
            printf("%.0f%% ", progress * 100.0f);
            fflush(stdout);
            progressUpdate = progress;
        }
    }

    printf("100%%\n\n");
    printf("✓ Test render complete!\n");
    printf("  Output file:   spectralblur_artifact_test.wav\n\n");

    printf("Next steps:\n");
    printf("  1. Open WAV in spectrogram viewer (e.g., Sonic Visualiser, Audacity)\n");
    printf("  2. Listen for zipper noise or clicks during Blur 0→100%% sweep\n");
    printf("  3. Check spectrogram for:\n");
    printf("     - Discontinuous frequency bands (indicate filter cutoff glitches)\n");
    printf("     - Harmonic aliasing in high-freq region (indicates poor alias resistance)\n");
    printf("     - Smooth spectral transition (good: blur opens gradually)\n");
    printf("  4. Listen for:\n");
    printf("     - Smooth parameter automation (no stepping or modulation artifacts)\n");
    printf("     - Clean output (no digital noise or distortion)\n");
    printf("     - Stable tail (no blow-up or instability after sweep ends)\n\n");

    printf("Artifact severity guide:\n");
    printf("  ✓ PASS:  Smooth sweep, no zippers, spectral continuity\n");
    printf("  ⚠ WARN:  Minor clicks at extreme settings, otherwise clean\n");
    printf("  ✗ FAIL:  Continuous zipper artifacts, aliasing, or instability\n\n");

    return 0;
}
