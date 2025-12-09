#include <SFML/Graphics.hpp>
#include "VictoryUI.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include "EntityManager.hpp"
#include <iostream>
#include <sstream>

VictoryUI::VictoryUI(sf::Font& font, const sf::RenderWindow& window)
    : winText(font), totalTime(font),
    playAgainButton({ 220.f, 80.f }, { 0.f, 0.f }, "Play Again", font),
    homeButton({ 220.f, 80.f }, { 0.f, 0.f }, "Return Home", font),
    exitButton({ 220.f, 80.f }, { 0.f, 0.f }, "Close Game", font)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    // setting text
    winText.setCharacterSize(48);
    winText.setString("You Win!");
    winText.setFillColor(sf::Color(255, 255, 255, 200));
    auto winTbounds = winText.getLocalBounds();
    winText.setOrigin({ winTbounds.size.x / 2, winTbounds.size.y / 2 });
    winText.setPosition({ screenWidth / 2.f, screenHeight / 4.f });

    totalTime.setCharacterSize(38);
    totalTime.setString("0.0");
    totalTime.setFillColor(sf::Color(255, 255, 255, 200));
    auto timeBounds = totalTime.getLocalBounds();
    totalTime.setOrigin({ timeBounds.size.x / 2, timeBounds.size.y / 2 });
    totalTime.setPosition({ screenWidth / 2.f, screenHeight * 5.f / 8.f });

    playAgainButton.setPosition({ (screenWidth / 2.f) , screenHeight * 2.2f / 5.f });
    playAgainButton.setCharSize(32);
    homeButton.setPosition({ (screenWidth / 2.f) , screenHeight * 3.f / 5.f });
    homeButton.setCharSize(32);
    exitButton.setPosition({ (screenWidth / 2.f) , screenHeight * 3.8f / 5.f });
    exitButton.setCharSize(32);
}

void VictoryUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();
    auto& entMan = EntityManager::getInstance();

    std::stringstream finalT;
    finalT << std::fixed << std::setprecision(2) << entMan.getTime();
    std::string timeStr = finalT.str();
    totalTime.setString(timeStr);

    bool playAgainHover = playAgainButton.isMouseOver(window);
    bool homeHover = homeButton.isMouseOver(window);
    bool exitHover = exitButton.isMouseOver(window);
    playAgainButton.setHover(playAgainHover);
    homeButton.setHover(homeHover);
    exitButton.setHover(exitHover);

    if (playAgainButton.isClicked(window)) {
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
    state.setState(GameStateManager::State::Victory);
    return;
}

void VictoryUI::render(sf::RenderWindow& window) {
    window.draw(winText);
    playAgainButton.render(window);
    homeButton.render(window);
    exitButton.render(window);
}
