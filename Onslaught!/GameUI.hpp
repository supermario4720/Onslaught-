#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"
#include "KeyIcon.hpp"

#include <vector>

class Camera;
class Town;
struct KeyIcon;
class InventoryUI;

class GameUI {
public:
    GameUI(sf::Font& font, sf::RenderWindow& window, Camera* camPtr);

    void update(float dt, sf::RenderWindow& window, InventoryUI& inventory);

    void render(sf::RenderWindow& window);

    void setHP(sf::Vector2f health);

    void setStamina(sf::Vector2f stamina);

    bool pauseClicked(sf::RenderWindow& window);

    void updateArrow();

    void checkTownOnScreen(sf::Vector2f& pos, sf::Vector2f& size);

    void setKeyboardTexture();

private:
    Camera* cameraPtr = nullptr;

    sf::RectangleShape hpBackground;
    sf::RectangleShape hpBar;
    sf::RectangleShape staminaBackground;
    sf::RectangleShape staminaBar;
    // player stats
    float currentHP;
    float maxHP;
    float currentStamina;
    float maxStamina;

    // arrow pointing to town
    sf::FloatRect townBounds;
    sf::Sprite townArrow;
    sf::Vector2f initialDir = {1.f, 0.f};
    float arrowOffset = 75.f;
    bool showArrow = true;
    std::weak_ptr<Town> townPtr;

    // Pause button (uses your Button class)
    Button pauseButton;
    Button inventoryButton;
    //maybe make get function for player health values instead for text
    sf::Text scoreText;
    sf::Text timeText;

    float screenWidth;
    float screenHeight;


    std::vector<KeyIcon> keyboardButtons;
};
