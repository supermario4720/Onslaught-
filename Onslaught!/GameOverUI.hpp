#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"


class GameOverUI {
public:
    GameOverUI(sf::Font& font, const sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

private:
    sf::Text gameOverText;

    // Pause button (uses your Button class)
    Button retryButton;
    Button homeButton;
    Button exitButton;

    float screenWidth;
    float screenHeight;
};
