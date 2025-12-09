#pragma once
/*
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Animation {
private:
	sf::Sprite sprite;

	int currentFrame;
	int state;
	float frameTimer;
	int totalFrames;        // How many frames in animation
	int frameWidth;        // Width of one frame
	int frameHeight;       // Height of one frame
	float frameSpeed;    // Seconds per frame
public:
    // In Player constructor
    Animation(const sf::Texture& texture, const int totalFrames = 1,
        const int frameWidth = 32, const int frameHeight = 32, const float frameSpeed = 0.1f);

	void setParameters(const int _totalFrames = 1, const int _frameWidth = 32,
		const int _frameHeight = 32, const float _frameSpeed = 0.1f);

	void update(float dt);

	void switchState(int _state);

	void setPosition(sf::Vector2f pos);

	void render(sf::RenderWindow& window);

	void resetFrame();
};
*/

// Animation.hpp
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
    Animation(const sf::Texture& texture,
        sf::Vector2u imageCount,
        unsigned int rowIndex,
        unsigned int frameStart,
        unsigned int frameEnd,
        float switchTime)
        : texture(&texture),
        imageCount(imageCount),
        row(rowIndex),
        frameStart(frameStart),
        frameEnd(frameEnd),
        switchTime(switchTime),
        totalTime(0.f)
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
