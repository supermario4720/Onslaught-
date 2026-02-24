#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <iostream>

class Animation {
public:
    Animation() = default;

    // imageCount = {columns, rows}, rowIndex = which row this animation uses
    Animation(const sf::Texture& texture, sf::Vector2u imageCount, unsigned int rowIndex,
        unsigned int frameStart, unsigned int frameEnd, float switchTime)
        : texture(&texture), imageCount(imageCount), row(rowIndex), frameStart(frameStart), frameEnd(frameEnd),
        switchTime(switchTime), totalTime(0.f)
    {
        currentFrame.x = frameStart;
        currentFrame.y = row;

        frameSize.x = texture.getSize().x / imageCount.x;
        frameSize.y = texture.getSize().y / imageCount.y;
    }

    // return if end of animation
    bool update(float dt, bool loop = true) {
        totalTime += dt;

        if (totalTime >= switchTime) {
            totalTime -= switchTime;
            currentFrame.x++;

            if (currentFrame.x > frameEnd) {
                if (loop) {
                    currentFrame.x = frameStart;
                }
                else {
                    currentFrame.x = frameEnd; // stay on last frame
                    return true;
                }
            }
        }
        return false;
    }

    void applyToSprite(sf::Sprite& sprite) const {
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(
            { static_cast<int>(currentFrame.x * frameSize.x),
             static_cast<int>(currentFrame.y * frameSize.y) },
            { static_cast<int>(frameSize.x),
            static_cast<int>(frameSize.y) }
        ));
    }

    void reset() {
        totalTime = 0.f;
        currentFrame.x = frameStart;
    }

private:
    const sf::Texture* texture = nullptr;

    sf::Vector2u imageCount;   // columns, rows
    sf::Vector2u frameSize;    // px
    sf::Vector2u currentFrame; // x = column, y = row

    unsigned int row = 0;
    unsigned int frameStart = 0;
    unsigned int frameEnd = 0;

    float switchTime = 0.1f;   // seconds per frame
    float totalTime = 0.f;
};
