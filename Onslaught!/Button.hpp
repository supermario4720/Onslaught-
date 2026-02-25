#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

//全てのUIに使われるボタンのクラス
class Button {
private:
    sf::RectangleShape box;
    sf::Text text;
    float textOffset;
    
    sf::Sprite sprite;
    float spriteOffset;

    bool isSquare;
    bool isMouseAlreadyOver;
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& textString, sf::Font& font);

    void render(sf::RenderWindow& window) const;

    bool isMouseOver(sf::RenderWindow& window);

    bool isClicked(sf::RenderWindow& window);

    void setHover(bool h);

    void setPosition(sf::Vector2f pos);

    void setCharSize(int size);

    void setTextOffset(float newOffset);

};
