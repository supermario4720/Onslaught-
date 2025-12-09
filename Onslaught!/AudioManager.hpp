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
    void loadSound(const std::string& name, const std::string& filepath) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filepath)) {
            // Handle file load error here
            return;
        }
        buffers[name] = std::move(buffer);
    }

    // Play one-shot sound (footstep, click, etc.)
    void play(const std::string& name, bool canPlayMultiple = false, float volume = 100.f) {
        auto it = buffers.find(name);
        if (it == buffers.end())
            return;

        const sf::SoundBuffer& buffer = it->second;

        if (!canPlayMultiple && isSoundPlaying(buffer)) {
            // Avoid starting duplicate overlapping sound
            return;
        }

        // Reuse an existing stopped sound if possible
        for (auto& s : sounds) {
            if (s.getStatus() == sf::Sound::Status::Stopped) {
                s.setBuffer(buffer);
                s.setVolume(volume);
                s.play();
                return;
            }
        }

        // Otherwise create a new one
        sf::Sound s(buffer);
        s.setVolume(volume);
        s.play();
        sounds.push_back(std::move(s));
    }

    // For longer-lived or repeatable sounds, create sf::Sound as a member of your class and do:
    // sf::Sound sound;
    // sound.setBuffer(AudioManager::getInstance().getBuffer("name"));
    // sound.play();

    // For advanced use: expose buffer directly if you want
    const sf::SoundBuffer& getBuffer(const std::string& name) const {
        auto it = buffers.find(name);
        if (it != buffers.end())
            return it->second;
        throw std::runtime_error("SoundBuffer not found!");
    }

    bool isSoundPlaying(const sf::SoundBuffer& buffer) {
        for (const auto& s : sounds) {
            if (&s.getBuffer() == &buffer && s.getStatus() == sf::Sound::Status::Playing) {
                return true; // Already playing this sound buffer
            }
        }
        return false;
    }

    void loadAllSounds();

private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;

    AudioManager() = default;
    // Delete copy/move so singleton can't be copied
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};