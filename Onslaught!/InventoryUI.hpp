#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

class InventoryUI {
public:
    InventoryUI(sf::Font& font, const sf::RenderWindow& window);

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

private:
    sf::RectangleShape background;
    sf::RectangleShape leftSide;
    sf::RectangleShape rightSide;
    sf::RectangleShape skillBackground;
    // list of inv slots?
    sf::RectangleShape inventorySlot;
    std::vector<sf::RectangleShape> slotShape;`

    // Close inv and return to game
    Button resumeButton;
    Button inventoryButton;
    Button skillButton;

    // Inv text
    sf::Text pauseText;

    float screenWidth;
    float screenHeight;
};
