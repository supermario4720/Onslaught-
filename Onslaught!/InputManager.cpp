#include "InputManager.hpp"
#include "Camera.hpp"
#include <algorithm> // std::copy
#include <iostream>

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager() {
    // SFML 3 provides KeyCount and ButtonCount in the enums
    const std::size_t keyCount = static_cast<std::size_t>(sf::Keyboard::KeyCount);
    const std::size_t mouseCount = static_cast<std::size_t>(sf::Mouse::ButtonCount);

    currentKeys.assign(keyCount, 0);
    previousKeys.assign(keyCount, 0);

    currentMouse.assign(mouseCount, 0);
    previousMouse.assign(mouseCount, 0);
}

void InputManager::setCamera(Camera& camera) {
    cameraPtr = &camera;
}

void InputManager::update(const sf::RenderWindow& window) {
    // store previous states
    std::copy(currentKeys.begin(), currentKeys.end(), previousKeys.begin());
    std::copy(currentMouse.begin(), currentMouse.end(), previousMouse.begin());

    const std::size_t keyCount = static_cast<std::size_t>(sf::Keyboard::KeyCount);
    const std::size_t mouseCount = static_cast<std::size_t>(sf::Mouse::ButtonCount);

    // update keyboard
    for (std::size_t i = 0; i < keyCount; ++i) {
        sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(i);
        currentKeys[i] = sf::Keyboard::isKeyPressed(k) ? 1 : 0;
    }

    // update mouse buttons
    for (std::size_t i = 0; i < mouseCount; ++i) {
        sf::Mouse::Button b = static_cast<sf::Mouse::Button>(i);
        currentMouse[i] = sf::Mouse::isButtonPressed(b) ? 1 : 0;
    }
}

bool InputManager::isKeyDown(sf::Keyboard::Key key) const {
    auto idx = static_cast<std::size_t>(key);
    assert(idx < currentKeys.size());
    return currentKeys[idx];
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const {
    auto idx = static_cast<std::size_t>(key);
    assert(idx < currentKeys.size());
    return currentKeys[idx] && !previousKeys[idx];
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const {
    auto idx = static_cast<std::size_t>(key);
    assert(idx < currentKeys.size());
    return !currentKeys[idx] && previousKeys[idx];
}

bool InputManager::isMouseDown(sf::Mouse::Button btn) const {
    auto idx = static_cast<std::size_t>(btn);
    assert(idx < currentMouse.size());
    return currentMouse[idx];
}

bool InputManager::isMousePressed(sf::Mouse::Button btn) const {
    auto idx = static_cast<std::size_t>(btn);
    assert(idx < currentMouse.size());
    return currentMouse[idx] && !previousMouse[idx];
}

bool InputManager::isMouseReleased(sf::Mouse::Button btn) const {
    auto idx = static_cast<std::size_t>(btn);
    assert(idx < currentMouse.size());
    return !currentMouse[idx] && previousMouse[idx];
}

sf::Vector2f InputManager::getMousePosition(const sf::RenderWindow& window) const {
    // SFML 3: getPosition(window) returns sf::Vector2i. Convert explicitly.
    sf::Vector2i posI = sf::Mouse::getPosition(window);
    return sf::Vector2f(static_cast<float>(posI.x), static_cast<float>(posI.y));
}

sf::Vector2f InputManager::getMouseWorldPosition(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = {0.f,0.f};
    if(cameraPtr != nullptr) {
        const sf::View view = (*cameraPtr).getView();
        worldPos = window.mapPixelToCoords(mousePos, view);
    }
    else {
        worldPos = window.mapPixelToCoords(mousePos);
    }
    return worldPos;
}


void InputManager::resetMouse() {
    for (int i = 0; i < 8; ++i) {
        previousMouse[i] = currentMouse[i]; // treat as if button was just released
    }
}

void InputManager::clear() {
    currentKeys.clear();
    previousKeys.clear();
    currentMouse.clear();
    previousMouse.clear();
    cameraPtr = nullptr;
}
