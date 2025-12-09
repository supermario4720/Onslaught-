#include "GameStateManager.hpp"
#include <iostream>

GameStateManager& GameStateManager::getInstance() {
    static GameStateManager instance;
    return instance;
}

void GameStateManager::setState(State newState) {
    currentState = newState;
}

GameStateManager::State GameStateManager::getState() const {
    return currentState;
}
