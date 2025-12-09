#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    static AudioManager& getInstance() {
        static AudioManager instance;
        return instance;
    }

    // Load sounds into the manager
    void loadSound(const std::string& name, const std::string& filepath);

    // Play one-shot sound (footstep, click, etc.)
    void play(const std::string& name, bool canPlayMultiple = false, float volume = 100.f);

    // For advanced use: expose buffer directly if you want
    const sf::SoundBuffer& getBuffer(const std::string& name) const;

    bool isSoundPlaying(const sf::SoundBuffer& buffer);

    void loadAllSounds();

    void clearManager();

private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;

    AudioManager() = default;
    ~AudioManager();
    // Delete copy/move so singleton can't be copied
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};