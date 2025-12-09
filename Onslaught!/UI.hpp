#pragma once
#include "InputManager.hpp"

class UI {
protected:
    InputManager& input;
public:
    UI()
    : input(InputManager::getInstance())
    { }
};