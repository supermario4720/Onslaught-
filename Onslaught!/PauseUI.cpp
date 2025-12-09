#include <SFML/Graphics.hpp>
#include "PauseUI.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include <iostream>

PauseUI::PauseUI(sf::Font& font, const sf::RenderWindow& window)
    : pauseText(font), 
    resumeButton({ 200.f, 80.f }, { 0.f, 0.f }, "Resume", font),
    homeButton({ 200.f, 80.f }, { 0.f, 0.f }, "Return Home", font),
    exitButton({ 200.f, 80.f }, { 0.f, 0.f }, "Close Game", font)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    pauseText = sf::Text(font);   // SFML 3 requires constructing with font
    pauseText.setCharacterSize(48);
    pauseText.setString("Paused");
    pauseText.setFillColor(sf::Color(255, 255, 255, 200));

    auto bounds = pauseText.getLocalBounds();
    pauseText.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    pauseText.setPosition( { screenWidth / 2.f, screenHeight / 4.f } );

    resumeButton.setPosition( { (screenWidth / 2.f) , screenHeight * 2.2f / 5.f } );  // Top-right
    resumeButton.setCharSize(32);
    homeButton.setPosition( { (screenWidth / 2.f) , screenHeight * 3.f / 5.f } );  // Top-right
    homeButton.setCharSize(30);
    exitButton.setPosition( { (screenWidth / 2.f) , screenHeight * 3.8f / 5.f } );  // Top-right
    exitButton.setCharSize(32);
}

void PauseUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();
    
    bool resumeHover = resumeButton.isMouseOver(window);
    bool homeHover = homeButton.isMouseOver(window);
    bool exitHover = exitButton.isMouseOver(window);
    resumeButton.setHover(resumeHover);
    homeButton.setHover(homeHover);
    exitButton.setHover(exitHover);

    if (resumeButton.isClicked(window)) {
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
    state.setState(GameStateManager::State::Pause);
    return;
}

void PauseUI::render(sf::RenderWindow& window) {
    window.draw(pauseText);
    resumeButton.render(window);
    homeButton.render(window);
    exitButton.render(window);
}
