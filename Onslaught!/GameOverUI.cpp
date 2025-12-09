#include <SFML/Graphics.hpp>
#include "GameOverUI.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include <iostream>

GameOverUI::GameOverUI(sf::Font& font, const sf::RenderWindow& window)
    : gameOverText(font),
    retryButton({ 220.f, 80.f }, { 0.f, 0.f }, "Retry", font),
    homeButton({ 220.f, 80.f }, { 0.f, 0.f }, "Return Home", font),
    exitButton({ 220.f, 80.f }, { 0.f, 0.f }, "Close Game", font)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    gameOverText = sf::Text(font);   // SFML 3 requires constructing with font
    gameOverText.setCharacterSize(48);
    gameOverText.setString("Game Over...");
    gameOverText.setFillColor(sf::Color(255, 255, 255, 200));
    auto bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    gameOverText.setPosition({ screenWidth / 2.f, screenHeight / 4.f });

    retryButton.setPosition({ (screenWidth / 2.f) , screenHeight * 2.2f / 5.f });  // Top-right
    retryButton.setCharSize(32);
    homeButton.setPosition({ (screenWidth / 2.f) , screenHeight * 3.f / 5.f });  // Top-right
    homeButton.setCharSize(32);
    exitButton.setPosition({ (screenWidth / 2.f) , screenHeight * 3.8f / 5.f });  // Top-right
    exitButton.setCharSize(32);
}

void GameOverUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();

    bool retryHover = retryButton.isMouseOver(window);
    bool homeHover = homeButton.isMouseOver(window);
    bool exitHover = exitButton.isMouseOver(window);
    retryButton.setHover(retryHover);
    homeButton.setHover(homeHover);
    exitButton.setHover(exitHover);

    if (retryButton.isClicked(window)) {
        state.setState(GameStateManager::State::Playing);
        return;
    }
    if (homeButton.isClicked(window)) {
        state.setState(GameStateManager::State::Title);
        return;
    }
    if (exitButton.isClicked(window)) {
        state.setState(GameStateManager::State::Exit);
        return;
    }
    state.setState(GameStateManager::State::GameOver);
    return;
}

void GameOverUI::render(sf::RenderWindow& window) {
    window.draw(gameOverText);
    retryButton.render(window);
    homeButton.render(window);
    exitButton.render(window);
}
