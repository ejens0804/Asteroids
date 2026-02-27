#pragma once
#include <SFML/Audio.hpp>
#include <array>

// Manages procedurally-generated sound effects.
// No external audio files required.
class SoundManager {
public:
    enum class Sound {
        SHOOT,
        EXPLODE_LARGE,
        EXPLODE_MEDIUM,
        EXPLODE_SMALL,
        PLAYER_DIE,
        THRUST,
        COUNT
    };

    static SoundManager& instance();

    void play(Sound s);

private:
    SoundManager();
    ~SoundManager() = default;

    static constexpr int NUM_SOUNDS = static_cast<int>(Sound::COUNT);

    // Buffers MUST outlive their sf::Sound objects
    std::array<sf::SoundBuffer, NUM_SOUNDS> buffers;
    std::array<sf::Sound,       NUM_SOUNDS> sounds;

    void generateAll();

    // Waveform builders
    sf::SoundBuffer makeTone(float freqStart, float freqEnd,
                             float duration, float volume,
                             bool noise = false) const;
};
