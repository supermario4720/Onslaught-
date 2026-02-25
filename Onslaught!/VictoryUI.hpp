#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

//ƒQ[ƒ€Ÿ—˜‚ÌUI
class VictoryUI {
public:
    VictoryUI(sf::Font& font, const sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

private:
    sf::Text winText;
    sf::Text totalTime;

    // Pause button (uses your Button class)
    Button playAgainButton;
    Button homeButton;
    Button exitButton;

    float screenWidth;
    float screenHeight;
};
