#include <SFML/Graphics.hpp>
#include "GameUI.hpp"
#include "TextureManager.hpp"
#include "Camera.hpp"
#include "GameStateManager.hpp"
#include "EntityManager.hpp"
#include "Town.hpp"
#include "InventoryUI.hpp"

#include <iostream>
#include <sstream>
#include <string>

GameUI::GameUI(sf::Font& font, sf::RenderWindow& window, Camera* camPtr)
    : scoreText(font), timeText(font), currentHP(0.f), maxHP(0.f), currentStamina(0.f), maxStamina(0.f),
    pauseButton({ 40.f, 40.f }, { 0.f, 0.f }, "||", font), inventoryButton({ 150.f, 40.f }, { 0.f, 0.f }, "Open Inventory", font),
    cameraPtr(camPtr), townArrow(TextureManager::getInstance().getTexture("Pointer"))
{
    townBounds = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    // ----- HP -----
    hpBackground.setSize({ screenWidth/3.f, screenHeight/20.f });
    hpBackground.setFillColor(sf::Color(80, 0, 0));
    hpBackground.setPosition({ 20.f, 20.f });
    hpBar.setSize({ screenWidth / 3.f, screenHeight / 20.f });
    hpBar.setFillColor(sf::Color(255, 0, 0));
    hpBar.setPosition({20.f, 20.f});

    // ----- Stamina -----
    staminaBackground.setSize({ screenWidth / 3.f, screenHeight / 20.f });
    staminaBackground.setFillColor(sf::Color(30, 30, 30));
    staminaBackground.setPosition({ 20.f, 30.f + screenHeight/20.f });
    staminaBar.setSize({ screenWidth / 3.f, screenHeight / 20.f });
    staminaBar.setFillColor(sf::Color(0, 200, 0));
    staminaBar.setPosition({ 20.f, 30.f + screenHeight / 20.f });

    // ----- Pause Button -----
    pauseButton.setPosition({screenWidth - 40.f, 40.f});  // Top-right
    pauseButton.setCharSize(20);

    // ----- Pause Button -----
    inventoryButton.setPosition({100, screenHeight/5.f});  // Top-right
    inventoryButton.setCharSize(18);
    inventoryButton.setTextOffset(5);

    // ----- Text -----
    scoreText.setCharacterSize(32);
    scoreText.setString("0");
    scoreText.setFillColor(sf::Color::White);
    auto scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin({ scoreBounds.size.x / 2, scoreBounds.size.y / 2 });
    scoreText.setPosition({ screenWidth / 2.f, 20.f });

    timeText.setCharacterSize(32);
    timeText.setString("0.0");
    timeText.setFillColor(sf::Color::White);
    auto timeBounds = timeText.getLocalBounds();
    timeText.setOrigin({ timeBounds.size.x / 2, timeBounds.size.y / 2 });
    timeText.setPosition({ (screenWidth - 140.f), 20.f });


    // setting up arrow to town
    sf::Vector2f arrowSize = townArrow.getGlobalBounds().size;
    townArrow.setOrigin({arrowSize.x / 2.f, arrowSize.y/2.f});

	float scale = (40.f / arrowSize.x);
	//townArrow.setScale({scale, scale});
    townArrow.setPosition({0.f, 0.f});

    setKeyboardTexture();
}

void GameUI::update(float dt, sf::RenderWindow& window, InventoryUI& inventory) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();
    EntityManager& entMan = EntityManager::getInstance();

    std::stringstream timeStream;
    float time = entMan.getTime();
    // change timer to red when past 2 minutes (wave start)
    if(time > 120.f) timeText.setFillColor(sf::Color(255, 5, 5, 235));
    else timeText.setFillColor(sf::Color(255, 255, 255, 235));
    
    timeStream << std::fixed << std::setprecision(2) << time;
    std::string timeStr = timeStream.str();
    std::string scoreStr = std::to_string(entMan.getScore());
    scoreText.setString(scoreStr);
    timeText.setString(timeStr);

    // set HP bar
    setHP(entMan.getPlayerHealth());
    setStamina(entMan.getPlayerStamina());

    updateArrow();
    for(KeyIcon& button : keyboardButtons) button.updateVisual(input);
 
    bool hover = pauseButton.isMouseOver(window);
    pauseButton.setHover(hover);
    hover = inventoryButton.isMouseOver(window);
    inventoryButton.setHover(hover);

    if (state.getState() == GameStateManager::State::GameOver) {
        return;
    }
    if (entMan.getTime() >= 180.f) {
        state.setState(GameStateManager::State::Victory);
        return;
    }

    if (pauseButton.isClicked(window)) {
        state.setState(GameStateManager::State::Pause);
        return;
    }
    if (inventoryButton.isClicked(window)) {
        inventory.setVisibility(true);
    }


    state.setState(GameStateManager::State::Playing);
    return;
}

void GameUI::render(sf::RenderWindow& window) {
    if(showArrow) {
        window.draw(townArrow);
    }
    for(KeyIcon& button : keyboardButtons) {
        //std::cout << "drawn" << std::endl;
        button.render(window);
    }
    window.draw(hpBackground);
    window.draw(hpBar);
    window.draw(staminaBackground);
    window.draw(staminaBar);
    window.draw(scoreText);
    window.draw(timeText);

    pauseButton.render(window);
    inventoryButton.render(window);
}

void GameUI::setHP(sf::Vector2f health) {
    currentHP = health.x;
    maxHP = health.y;
    float percent = currentHP / maxHP;
    hpBar.setSize({ (screenWidth/3.f)*percent, screenHeight/20.f });
}

void GameUI::setStamina(sf::Vector2f stamina) {
    currentStamina = stamina.x;
    maxStamina = stamina.y;
    float percent = currentStamina / maxStamina;
    staminaBar.setSize({ (screenWidth / 3.f) * percent, screenHeight / 20.f });
}

bool GameUI::pauseClicked(sf::RenderWindow& window) {
    return pauseButton.isClicked(window);
}

void GameUI::updateArrow() {
    const sf::View& view = cameraPtr->getView();
    sf::Vector2f viewPos = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    checkTownOnScreen(viewPos, viewSize);

    if(!showArrow) return;

    auto town = townPtr.lock();
    if(!town->checkAlive()) return;
    if(town->wasDamaged()) {
        townArrow.setColor(sf::Color(255, 100, 100, 255));
    }
    else {
        townArrow.setColor(sf::Color(255, 255, 255, 255));
    }

    sf::Angle dir = initialDir.angleTo( -viewPos );
    sf::Vector2f arrowOffsetPos(arrowOffset, dir);
    sf::Vector2f arrowPos = arrowOffsetPos + viewSize/2.f;

    townArrow.setRotation(dir);
    townArrow.setPosition(arrowPos);
}


void GameUI::checkTownOnScreen(sf::Vector2f& pos, sf::Vector2f& size) {
    if(townPtr.expired()) townPtr = EntityManager::getInstance().getTownPtr();
    if(townBounds.size.x == 0.f || townBounds.size.y == 0.f) {
        auto town = townPtr.lock();
        townBounds = town->getBounds();
    }
    sf::FloatRect viewRect( {pos - size/2.f}, size );
    showArrow = !(viewRect.findIntersection(townBounds));
}


void GameUI::setKeyboardTexture() {
    auto& keyboardTexture = TextureManager::getInstance().getTexture("KeyboardButtons");
    auto& keyboardExtraTexture = TextureManager::getInstance().getTexture("KeyboardExtraButtons");
    keyboardButtons.clear();

    float baseY = screenHeight - 60.f;
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::W, 6, 4, { 80.f,  baseY - 40.f }, false) );
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::A, 0, 2, { 40.f,  baseY }, false) );
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::S, 2, 4, { 80.f,  baseY }, false) );
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::D, 3, 2, { 120.f,  baseY }, false) );
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::E, 4, 2, { 340.f,  baseY }, false) );
    keyboardButtons.push_back( KeyIcon(keyboardTexture, sf::Keyboard::Key::R, 1, 4, { 400.f,  baseY }, false) );


    keyboardButtons.push_back( KeyIcon(keyboardExtraTexture, sf::Keyboard::Key::Space, 2, 2, {500.f,  baseY }, true) );
    keyboardButtons.push_back( KeyIcon(keyboardExtraTexture, sf::Keyboard::Key::LShift, 0, 1, {600.f,  baseY }, true) );
}
