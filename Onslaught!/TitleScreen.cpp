#include <SFML/Graphics.hpp>
#include "TitleScreen.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"



TitleScreen::TitleScreen(sf::Font& font, sf::RenderWindow& window)
    : title(font), titleTexture("resources/Onslaught_Logo-BGremoved.png"),
    sprite(titleTexture),
    playButton({ 300, 70 }, { 340, 300 }, "PLAY", font),
    exitButton({ 300, 70 }, { 340, 400 }, "EXIT", font)
{
    float windowWidth = (float)window.getSize().x;
    float windowHeight = (float)window.getSize().y;

    // Game title
    title.setCharacterSize(64);
    title.setString("Onslaught!");
    title.setFillColor(sf::Color::White);
    auto bounds = title.getLocalBounds();
    title.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    title.setPosition({ windowWidth/2.f, windowHeight/4.f });

    fadedBackground.setSize({windowWidth, windowHeight/4.f});
    fadedBackground.setOrigin({ windowWidth/2.f, windowHeight / 9.f });
    fadedBackground.setPosition({ windowWidth / 2.f, (windowHeight / 4.f) - 10.f });
    fadedBackground.setFillColor(sf::Color(0, 0, 0, 100)); // 150 = semi-transparent black


    sprite.setTexture(titleTexture);
    sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
    sprite.setOrigin({ spriteSize.x / 2.f, spriteSize.y / 2.f });
    // scale so width of logo is half width of screen
    float scale = windowWidth * 0.75f / spriteSize.x;
    sprite.setScale({ scale , scale });
    sprite.setPosition({ windowWidth / 2.f, windowHeight / 4.f });
    sprite.rotate(sf::degrees(2.f));

    // Buttons
    playButton.setPosition({ windowWidth / 2.f, windowHeight * 3.f / 5.f });
    exitButton.setPosition({ windowWidth / 2.f, windowHeight * 4.f / 5.f });
}

void TitleScreen::update(sf::RenderWindow& window) {

    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();

    // Keyboard navigation
    if (input.isKeyPressed(sf::Keyboard::Key::Up)) {
        selectedIndex--;
    }
    if (input.isKeyPressed(sf::Keyboard::Key::Down)) {
        selectedIndex++;
    }

    if (selectedIndex < 0) selectedIndex = 1;      // wrap around (2 buttons)
    if (selectedIndex > 1) selectedIndex = 0;

    // Mouse should also highlight buttons
    bool hoverPlay = playButton.isMouseOver(window);
    bool hoverExit = exitButton.isMouseOver(window);

    if (hoverPlay) {hoverExit = false; selectedIndex = 0;}
    if (hoverExit) {hoverPlay = false; selectedIndex = 1;}

    // Highlight either by hover OR keyboard index
    playButton.setHover(hoverPlay || selectedIndex == 0);
    exitButton.setHover(hoverExit || selectedIndex == 1);

    // Mouse click
    if (playButton.isClicked(window)) {
        state.setState(GameStateManager::State::Playing);
        return;
    }
    if (exitButton.isClicked(window)) {
        state.setState(GameStateManager::State::Exit);
        return;
    }

    // ENTER key activates selectedIndex
    if (input.isKeyPressed(sf::Keyboard::Key::Enter)) {
        if (selectedIndex == 0) {
            state.setState(GameStateManager::State::Playing);
            return;
        }
        if (selectedIndex == 1) {
            state.setState(GameStateManager::State::Exit);
            return;
        }
    }
    state.setState(GameStateManager::State::Title);
    return;
}


void TitleScreen::render(sf::RenderWindow& window) {
    //window.draw(title);
    window.draw(fadedBackground);
    window.draw(sprite);
    playButton.render(window);
    exitButton.render(window);
}
