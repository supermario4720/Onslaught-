#pragma once
#include "Experience.hpp"
#include <vector>

class Player;

// Player stats stored here
struct PlayerStatus {
    float maxHealth = 100.f;
    float healthRecoverySpeed = 5.f;
    float maxStamina = 100.f;
    float staminaRecoverySpeed = 50.f;
    float attackDamage = 20.f;
    float movementSpeed = 80.f;
};

class PlayerStatusManager {
public:
    PlayerStatusManager();

    void addExperience(int qty);

    void reset();

    void printExplvl() const;

    void levelUp();

    void setNextLevelReq();

    void updatePlayerStatus();

    void setPlayerPtr(std::weak_ptr<Player> ptr);

private:
    std::weak_ptr<Player> playerPtr;
    // Player stat growth rates
    float healthIncrem;
    float healthRecoveryIncremRate;
    float staminaIncrem;
    float staminaRecoveryIncremRate;
    float damageIncrem;
    float moveSpeedIncrem;

    PlayerStatus stats;
    // Player stats managed here
    float maxHealth;
    // health recovery per second
    float healthRecoverySpeed;
    float maxStamina;
    float staminaRecoverySpeed;
    float attackDamage;
    float movementSpeed;

    int totalExperience;
    int level;
    int toNextLevel;
};
