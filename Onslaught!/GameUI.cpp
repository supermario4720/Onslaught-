#include <SFML/Graphics.hpp>
#include "GameUI.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "EntityManager.hpp"

#include <iostream>
#include <sstream>
#include <string>

GameUI::GameUI(sf::Font& font, sf::RenderWindow& window)
    : scoreText(font), timeText(font), currentHP(0.f), maxHP(0.f), currentStamina(0.f), maxStamina(0.f),
    pauseButton({ 40.f, 40.f }, { 0.f, 0.f }, "||", font)
{
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

}

void GameUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();
    EntityManager& entMan = EntityManager::getInstance();

    std::stringstream timeStream;
    timeStream << std::fixed << std::setprecision(2) << entMan.getTime();
    std::string timeStr = timeStream.str();

    std::string scoreStr = std::to_string(entMan.getScore());

    scoreText.setString(scoreStr);
    timeText.setString(timeStr);

    // set HP bar
    setHP(entMan.getPlayerHealth());
    setStamina(entMan.getPlayerStamina());

    bool hover = pauseButton.isMouseOver(window);
    pauseButton.setHover(hover);

    if (state.getState() == GameStateManager::State::GameOver) {
        return;
    }
    if (entMan.getTime() >= 60.f) {
        state.setState(GameStateManager::State::Victory);
        return;
    }

    if (pauseButton.isClicked(window)) {
        state.setState(GameStateManager::State::Pause);
        return;
    }

    state.setState(GameStateManager::State::Playing);
    return;
}

void GameUI::render(sf::RenderWindow& window) {
    window.draw(hpBackground);
    window.draw(hpBar);
    window.draw(staminaBackground);
    window.draw(staminaBar);
    window.draw(scoreText);
    window.draw(timeText);

    pauseButton.render(window);
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
