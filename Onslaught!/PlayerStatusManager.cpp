#include "PlayerStatusManager.hpp"
#include "Player.hpp"
#include <iostream>

PlayerStatusManager::PlayerStatusManager()
: healthIncrem(10.f), healthRecoveryIncremRate(0.5f), staminaIncrem(10.f), staminaRecoveryIncremRate(0.5f),
damageIncrem(1.f), moveSpeedIncrem(5.f), stats(PlayerStatus())
{
    totalExperience = 0;
    level = 1;
    toNextLevel = 10;
}

void PlayerStatusManager::addExperience(int qty) {
    totalExperience += qty;

    int remainingQty = qty;

    while (remainingQty > 0) {
        int addedExp = std::min(remainingQty, toNextLevel);
        remainingQty -= addedExp;
        toNextLevel -= addedExp;
        if(toNextLevel == 0) levelUp();
    }
    updatePlayerStatus();
}

void PlayerStatusManager::reset() {
    totalExperience = 0;
    level = 1;

    stats = PlayerStatus();

    healthIncrem = 10.f;
    healthRecoveryIncremRate = 0.5f;
    staminaIncrem = 10.f;
    staminaRecoveryIncremRate = 0.5f;
    damageIncrem = 1.f;
    
    toNextLevel = 10;
}

void PlayerStatusManager::printExplvl() const {
    std::cout << level << " : " <<  totalExperience << std::endl;
    // status
    std::cout << "Health : " <<  stats.maxHealth << std::endl;
    std::cout << "Health Recovery : " <<  stats.healthRecoverySpeed << std::endl;
    std::cout << "Stamina : " <<  stats.maxStamina << std::endl;
    std::cout << "Stamina Recovery : " <<  stats.staminaRecoverySpeed << std::endl;
    std::cout << "Attack Damage : " <<  stats.attackDamage << std::endl;
    std::cout << "Movement Speed : " <<  stats.movementSpeed << std::endl;
}


void PlayerStatusManager::levelUp() {
    level += 1;
    stats.maxHealth += healthIncrem;
    stats.healthRecoverySpeed += healthRecoveryIncremRate;
    stats.maxStamina += staminaIncrem;
    stats.staminaRecoverySpeed += staminaRecoveryIncremRate;
    stats.attackDamage += damageIncrem;

    if (level % 3 == 0) stats.movementSpeed += moveSpeedIncrem;
    setNextLevelReq();
    std::cout << "Level UP! " << std::endl;
    printExplvl();
}

void PlayerStatusManager::setNextLevelReq() {
    // make code or method to determine exp required to next level (e.g. lvl * 3exp)
    toNextLevel = 10;
}

void PlayerStatusManager::updatePlayerStatus() {
    if(playerPtr.expired()) return;

    auto player = playerPtr.lock();
    player -> setHealth(stats.maxHealth);
    player -> setHealthRecoverRate(stats.healthRecoverySpeed);
    player -> setStamina(stats.maxStamina);
    player -> setStaminaRevoverRate(stats.staminaRecoverySpeed);
    player -> setDamage(stats.attackDamage);
    player -> setMoveSpeed(stats.movementSpeed);
}

void PlayerStatusManager::setPlayerPtr(std::weak_ptr<Player> ptr) {
    playerPtr = ptr;
    updatePlayerStatus();
}