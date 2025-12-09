#include "AudioManager.hpp"
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