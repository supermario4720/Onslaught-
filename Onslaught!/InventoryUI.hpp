#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

class InventoryManager;

class InventoryUI {
public:
    InventoryUI(sf::Font& font, const sf::RenderWindow& window);
    ~InventoryUI();

    void update(float dt, sf::RenderWindow& window);

    void render(sf::RenderWindow& window, const InventoryManager& inventoryManager);

    void setVisibility(bool open);

    bool isInventoryOpen() const;

private:
    // used to dim the background
    sf::RectangleShape background;
    // the inventory menu itself
    sf::RectangleShape inventoryBase;

    // Close inv and return to game
    Button resumeButton;

    // Inv text
    sf::Text inventoryText;

    bool inventoryOpen;

    float screenWidth;
    float screenHeight;
};
