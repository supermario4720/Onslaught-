#include "AudioManager.hpp"

AudioManager::~AudioManager() {
	buffers.clear();
	sounds.clear();
}


// Load sounds into the manager
void AudioManager::loadSound(const std::string& name, const std::string& filepath) {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filepath)) {
		// Handle file load error here
		return;
	}
	buffers[name] = std::move(buffer);
}

// Play one-shot sound (footstep, click, etc.)
void AudioManager::play(const std::string& name, bool canPlayMultiple, float volume) {
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

// For advanced use: expose buffer directly if you want
const sf::SoundBuffer& AudioManager::getBuffer(const std::string& name) const {
	auto it = buffers.find(name);
	if (it != buffers.end())
		return it->second;
	throw std::runtime_error("SoundBuffer not found!");
}

bool AudioManager::isSoundPlaying(const sf::SoundBuffer& buffer) {
	for (const auto& s : sounds) {
		if (&s.getBuffer() == &buffer && s.getStatus() == sf::Sound::Status::Playing) {
			return true; // Already playing this sound buffer
		}
	}
	return false;
}



// Load all sounds somewhere at game start
void AudioManager::loadAllSounds() {
	loadSound("ButtonClick", "resources/sounds/01_chest_open_2.wav");
	loadSound("ButtonHover", "resources/sounds/16_human_walk_stone_1.wav");

	loadSound("playerWalkFootstep", "resources/sounds/04_sack_open_2.wav");
	loadSound("playerDashFootstep", "resources/sounds/04_sack_open_3.wav");
	loadSound("playerAttack", "resources/sounds/12_human_jump_3.wav");
	loadSound("playerHurt", "resources/sounds/11_human_damage_3.wav");
	loadSound("playerDeath", "resources/sounds/14_human_death_spin.wav");

	//loadSound("enemyFootstep", "resources/sounds/footstep.wav");
	loadSound("enemyAttack", "resources/sounds/18_orc_charge.wav");
	loadSound("enemyHurt", "resources/sounds/17_orc_atk_sword_2.wav");
	loadSound("enemyDeath", "resources/sounds/20_orc_special_atk.wav");

	loadSound("townHurt", "resources/sounds/02_chest_close_2.wav");
	loadSound("townDestroy", "resources/sounds/02_chest_close_2.wav");

}

void AudioManager::clearManager() {
	buffers.clear();
	sounds.clear();
}