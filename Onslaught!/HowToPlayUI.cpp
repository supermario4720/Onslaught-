#include <SFML/Graphics.hpp>
#include "HowToPlayUI.hpp"
#include "InputManager.hpp"

HowToPlayUI::HowToPlayUI(sf::Font& font, const sf::RenderWindow& window)
    : closeButton({ 200.f, 80.f }, { 0.f, 0.f }, "Close", font),
    manualTexture("resources/WASD_Manual_Japanese_BGremoved.png"),
    manual(manualTexture)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    manual.setTexture(manualTexture);
    sf::Vector2f spriteSize = manual.getGlobalBounds().size;
    manual.setOrigin({ spriteSize.x / 2.f, spriteSize.y / 2.f });
    // scale so space for close button
    float scale = screenHeight * 0.78f / spriteSize.y;
    manual.setScale({ scale , scale });
    manual.setPosition({ screenWidth/2.f, screenHeight*0.43f});


    closeButton.setPosition({ (screenWidth / 2.f) , screenHeight * 4.6f / 5.f });  // Top-right
    closeButton.setCharSize(32);
}
// returns true if close is clicked
bool HowToPlayUI::update(sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();

    bool closeHover = closeButton.isMouseOver(window);
    closeButton.setHover(closeHover);

    if (closeButton.isClicked(window)) {
        return true;
    }
    return false;
}

void HowToPlayUI::render(sf::RenderWindow& window) {
    window.draw(manual);
    closeButton.render(window);
}
