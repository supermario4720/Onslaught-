/*
    Using the lerp function for smooth camera movement
    smoothing value = 0.001f;
*/

#pragma once
#include "SFML/Graphics.hpp"

class Camera {
public:
    // constructor function
    Camera(sf::RenderWindow& window, float width, float height);

    void follow(const sf::Vector2f& targetPos);

    void apply();

    sf::View getView() const;

    sf::Vector2f getPos() const;

private:
    sf::RenderWindow& renderWindow;
    sf::View cameraView;
    float cameraDeadzone;
    float smoothing = 0.1f;
};