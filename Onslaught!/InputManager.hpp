#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <cstdint>
#include <cassert>

class Camera;

class InputManager {
public:
    static InputManager& getInstance();

    void setCamera(Camera& camera);
    // Note: takes const ref to window; we don't store it
    void update(const sf::RenderWindow& window);

    bool isKeyDown(sf::Keyboard::Key key) const;
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isKeyReleased(sf::Keyboard::Key key) const;

    bool isMouseDown(sf::Mouse::Button btn) const;
    bool isMousePressed(sf::Mouse::Button btn) const;
    bool isMouseReleased(sf::Mouse::Button btn) const;

    sf::Vector2f getMousePosition(const sf::RenderWindow& window) const;
    sf::Vector2f getMouseWorldPosition(const sf::RenderWindow& window) const;

    void resetMouse();

    void clear();

private:
    InputManager();
    Camera* cameraPtr = nullptr;

    // sized at runtime from SFML's KeyCount / ButtonCount
    std::vector<char> currentKeys;
    std::vector<char> previousKeys;

    std::vector<char> currentMouse;
    std::vector<char> previousMouse;
};
