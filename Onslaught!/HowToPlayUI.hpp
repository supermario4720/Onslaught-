#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"


class HowToPlayUI {
public:
    HowToPlayUI(sf::Font& font, const sf::RenderWindow& window);
    // return true if close is clicked
    bool update(sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

private:
    Button closeButton;
    sf::Texture manualTexture;
    sf::Sprite manual;

    float screenWidth;
    float screenHeight;
};
