#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"


class GameUI {
public:
    GameUI(sf::Font& font, sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

    void setHP(sf::Vector2f health);

    void setStamina(sf::Vector2f stamina);

    bool pauseClicked(sf::RenderWindow& window);

private:
    // HP UI 
    sf::RectangleShape hpBackground;
    sf::RectangleShape hpBar;

    // Stamina UI
    sf::RectangleShape staminaBackground;
    sf::RectangleShape staminaBar;

    // Pause button (uses your Button class)
    Button pauseButton;

    //maybe make get function for player health values instead for text
    sf::Text scoreText;
    sf::Text timeText;

    float screenWidth;
    float screenHeight;

    // player stats
    float currentHP;
    float maxHP;

    float currentStamina;
    float maxStamina;

};
