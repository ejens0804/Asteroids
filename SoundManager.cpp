#include "SoundManager.h"
#include "Global.h"
#include <cmath>
#include <vector>
#include <random>

static std::mt19937 rng_s(42);

SoundManager& SoundManager::instance() {
    static SoundManager inst;
    return inst;
}

SoundManager::SoundManager() {
    generateAll();
}

// ─── Waveform generator ──────────────────────────────────────────────────────
sf::SoundBuffer SoundManager::makeTone(float freqStart, float freqEnd,
                                       float duration, float volume,
                                       bool addNoise) const
{
    const unsigned RATE = 44100;
    int nSamples = static_cast<int>(RATE * duration);
    std::vector<sf::Int16> samples(nSamples);
    std::uniform_real_distribution<float> noiseDist(-1.f, 1.f);

    for (int i = 0; i < nSamples; ++i) {
        float t   = static_cast<float>(i) / RATE;
        float pct = static_cast<float>(i) / nSamples;
        float env = (1.f - pct) * (1.f - pct); // quadratic decay

        float freq = freqStart + (freqEnd - freqStart) * pct;
        float wave = std::sin(2.f * PI * freq * t);

        if (addNoise)
            wave = wave * 0.4f + noiseDist(rng_s) * 0.6f;

        samples[i] = static_cast<sf::Int16>(32767 * volume * env * wave);
    }

    sf::SoundBuffer buf;
    buf.loadFromSamples(samples.data(), samples.size(), 1, RATE);
    return buf;
}

void SoundManager::generateAll() {
    // SHOOT  – short rising chirp
    buffers[0] = makeTone(600.f, 1200.f, 0.12f, 0.35f, false);

    // EXPLODE_LARGE  – low rumble + noise
    buffers[1] = makeTone(80.f,  30.f,  0.55f, 0.8f,  true);

    // EXPLODE_MEDIUM
    buffers[2] = makeTone(160.f, 60.f,  0.35f, 0.65f, true);

    // EXPLODE_SMALL  – high crack
    buffers[3] = makeTone(320.f, 120.f, 0.20f, 0.5f,  true);

    // PLAYER_DIE  – descending wail
    buffers[4] = makeTone(500.f,  50.f,  0.80f, 0.9f,  true);

    // THRUST  – smooth rocket engine: filtered noise + low rumble harmonics
    {
        const unsigned RATE = 44100;
        const float    DUR  = 0.22f;
        const int      N    = static_cast<int>(RATE * DUR);

        std::vector<float> raw(N);
        std::uniform_real_distribution<float> noiseDist(-1.f, 1.f);

        // AI was very helpful in writing the mathematical formulas for this part
        // --- Pass 1: generate raw white noise + tones ---
        for (int i = 0; i < N; ++i) {
            float t = static_cast<float>(i) / RATE;

            float fundamental = std::sin(2.f * PI *  55.f * t) * 0.35f;
            float harmonic2   = std::sin(2.f * PI * 110.f * t) * 0.18f;
            float harmonic3   = std::sin(2.f * PI * 165.f * t) * 0.08f;
            float wobble      = 0.75f + 0.25f * std::sin(2.f * PI * 8.f * t);
            float noise       = noiseDist(rng_s) * 0.60f;

            raw[i] = (fundamental + harmonic2 + harmonic3) * wobble + noise;
        }

        // --- Pass 2: multi-stage one-pole low-pass filter on the noise ---
        // Each stage smooths out high-frequency roughness
        // Cutoff coefficient: lower = smoother (0.04 ≈ ~280 Hz cutoff at 44100)
        const float LP = 0.04f;
        float state = 0.f;
        for (int pass = 0; pass < 3; ++pass) {       // 3 cascaded stages
            state = 0.f;
            for (int i = 0; i < N; ++i) {
                state += LP * (raw[i] - state);
                raw[i] = state;
            }
        }

        // --- Pass 3: apply envelope + tanh soft clip → convert to int16 ---
        std::vector<sf::Int16> samples(N);
        for (int i = 0; i < N; ++i) {
            float pct = static_cast<float>(i) / N;

            // Cubic ease-in / ease-out envelope (much smoother than linear)
            float env = 1.f;
            if (pct < 0.15f) {
                float x = pct / 0.15f;
                env = x * x * (3.f - 2.f * x);   // smoothstep
            } else if (pct > 0.75f) {
                float x = (pct - 0.75f) / 0.25f;
                env = 1.f - x * x * (3.f - 2.f * x);
            }

            float mixed = std::tanh(raw[i] * 1.8f) * 0.50f * env;
            samples[i]  = static_cast<sf::Int16>(mixed * 32767.f);
        }

        buffers[5].loadFromSamples(samples.data(), samples.size(), 1, RATE);
    }

    for (int i = 0; i < NUM_SOUNDS; ++i) {
        sounds[i].setBuffer(buffers[i]);
    }
}

void SoundManager::play(Sound s) {
    int idx = static_cast<int>(s);
    sounds[idx].stop();
    sounds[idx].play();
}