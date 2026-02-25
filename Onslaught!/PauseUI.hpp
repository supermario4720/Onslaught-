#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

//　ポーズ画面のUI
class PauseUI {
public:
    PauseUI(sf::Font& font, const sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

private:
    sf::Text pauseText;

    // Pause button (uses your Button class)
    Button resumeButton;
    Button homeButton;
    Button exitButton;

    float screenWidth;
    float screenHeight;
};
