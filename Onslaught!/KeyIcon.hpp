#pragma once
#include "InputManager.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

struct KeyIcon {
    sf::Sprite sprite;
    sf::Keyboard::Key key;
    sf::IntRect releasedRect;
    sf::IntRect pressedRect;

    int width = 16;
    int longWidth = 32;
    int height = 16;

    KeyIcon(const sf::Texture& tex, sf::Keyboard::Key k, int column, int row, sf::Vector2f pos, bool isLong = false )
    : sprite(tex), key(k)
    {  
        if(isLong) {
            releasedRect = sf::IntRect({column * longWidth, row * height}, {longWidth, height});
            pressedRect = sf::IntRect({column * longWidth, (row+4) * height}, {longWidth, height});
        }
        else {
            releasedRect = sf::IntRect({column * width, row * height}, {width, height});
            pressedRect = sf::IntRect({column * width, (row+7) * height}, {width, height});
        }
        sprite.setTextureRect(releasedRect);
        sprite.setOrigin({releasedRect.size.x / 2.f, releasedRect.size.y / 2.f});
        sf::Vector2f size = sprite.getGlobalBounds().size;
        float scale = 40.f / size.y;
        sprite.setScale({scale, scale});

        sprite.setPosition(pos);
        sprite.setColor(sf::Color(255, 255, 255, 175));
    }

    void updateVisual(const InputManager& input) {
        bool down = input.isKeyDown(key);
        sprite.setTextureRect(down ? pressedRect : releasedRect);
    }

    void render(sf::RenderWindow& window) const {
        window.draw(sprite);
    }
};
