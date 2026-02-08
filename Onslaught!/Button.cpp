#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "InputManager.hpp"
#include "AudioManager.hpp"
#include "TextureManager.hpp"
#include <iostream>


Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& textString, sf::Font& font)
    :text(font), sprite(TextureManager::getInstance().getTexture("Button")), isMouseAlreadyOver(false)
{
    box.setSize(size);
    box.setOrigin(size / 2.f);
    box.setPosition(position);
    box.setFillColor(sf::Color(100, 100, 255)); // default

    if (size.x == size.y) isSquare = true;
    else isSquare = false;

    TextureManager& textures = TextureManager::getInstance();

    sprite.setTexture(textures.getTexture("Button"));
    if (isSquare) {
        sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
        sprite.setOrigin({spriteSize.x / 2.f, spriteSize.y/2.f});

        float scaleX = (size.x * 1.2f / spriteSize.x);
        float scaleY = (size.y * 1.2f) / spriteSize.y;
        sprite.setScale({scaleX, scaleY});

        textOffset = 7.f;
    }
    else {
        const sf::Texture& tex = textures.getTexture("LongButton");
        sf::Vector2u texSize = tex.getSize(); // This is the actual size in pixels

        // Now use this for scaling
        float scaleX = (size.x * 1.1f) / static_cast<float>(texSize.x);
        float scaleY = (size.y * 1.1f) / static_cast<float>(texSize.y);
        
        sprite.setTexture(tex);
        sprite.setTextureRect(sf::IntRect({0,0},{static_cast<int>(texSize.x), static_cast<int>(texSize.y)}));
        sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
        sprite.setOrigin({spriteSize.x / 2.f, spriteSize.y/2.f});
        sprite.setScale({scaleX, scaleY});
        textOffset = 15.f;
    }

    spriteOffset = 5.f;
    sprite.setPosition({position.x, position.y + spriteOffset});

    text.setString(textString);
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);

    // center text inside button
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    text.setPosition( { position.x , position.y - textOffset } );
}

void Button::render(sf::RenderWindow& window) const {
    // window.draw(box);
    window.draw(sprite);
    window.draw(text);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2f mp = window.mapPixelToCoords( sf::Mouse::getPosition(window) );
    bool mouseOver = box.getGlobalBounds().contains(mp);
    if (mouseOver && !isMouseAlreadyOver) {
        AudioManager::getInstance().play("ButtonHover", true, 50.f);
        isMouseAlreadyOver = true;
    }
    else if (!mouseOver) {
        isMouseAlreadyOver = false;
    }

    return mouseOver;
}

bool Button::isClicked(sf::RenderWindow& window) {
    InputManager& input = InputManager::getInstance();
    TextureManager& textures = TextureManager::getInstance();

    bool clicked = (isMouseOver(window) && input.isMousePressed(sf::Mouse::Button::Left));
    if (clicked) {
        AudioManager::getInstance().play("ButtonClick", true, 80.f);
        if(isSquare) {
            sprite.setTexture(textures.getTexture("ButtonPressed"));
        }
        else {
            sprite.setTexture(textures.getTexture("LongButtonPressed"));
        }
    }
    return clicked;
}

void Button::setHover(bool h) {
    TextureManager& textures = TextureManager::getInstance();
    if (h) {
        box.setFillColor(sf::Color(150, 150, 255));
        if(isSquare) {
            sprite.setTexture(textures.getTexture("ButtonHover"));
        }
        else {
            sprite.setTexture(textures.getTexture("LongButtonHover"));
        }
    }
    else {
        box.setFillColor(sf::Color(100, 100, 255));
        if(isSquare) {
            sprite.setTexture(textures.getTexture("Button"));
        }
        else {
            sprite.setTexture(textures.getTexture("LongButton"));
        }
    }
}

void Button::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    sprite.setPosition({pos.x, pos.y + spriteOffset} );
    text.setPosition( { pos.x, pos.y - textOffset } );
}

void Button::setCharSize(int size) {
    sf::Vector2f pos = text.getPosition();

    text.setCharacterSize(size);
    // fix origin of text
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });

    text.setPosition( pos );
}

void Button::setTextOffset(float newOffset) {
    sf::Vector2f pos = text.getPosition();
    text.setPosition({pos.x, pos.y + textOffset - newOffset});
    textOffset = newOffset;
}