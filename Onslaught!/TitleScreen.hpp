#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Button.hpp"

class TitleScreen {
public:
    TitleScreen(sf::Font& font, sf::RenderWindow& window);

    void update(sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

private:
    sf::Text title;
    sf::Texture titleTexture;
    sf::Sprite sprite;

    sf::RectangleShape fadedBackground; 

    Button playButton;
    Button exitButton;

    int selectedIndex = 0;   // 0 = play, 1 = exit
};
