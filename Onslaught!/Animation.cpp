#include <SFML/Graphics.hpp>
#include "Animation.hpp"

/*
Animation::Animation(const sf::Texture& texture, const int _totalFrames,
    const int _frameWidth, const int _frameHeight, const float _frameSpeed)
: sprite(texture), totalFrames(_totalFrames), frameWidth(_frameWidth), frameHeight(_frameWidth),
frameSpeed(_frameSpeed), frameTimer(0.f), currentFrame(0), state(0)
{
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { frameWidth, frameHeight })); // Start at first frame
}

void Animation::setParameters(const int _totalFrames, const int _frameWidth,
    const int _frameHeight, const float _frameSpeed) {
    const int totalFrames = _totalFrames;
    const int frameWidth = _frameWidth;
    const int frameHeight = _frameHeight;
    const float frameSpeed = _frameSpeed;

    sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });
}

void Animation::update(float dt) {
    frameTimer += dt;
    if (frameTimer >= frameSpeed) {
        frameTimer = 0.f;
        currentFrame = (currentFrame + 1) % totalFrames; // Loop back to 0 after last frame

        // This is the "magic" - just shift the rectangle
        sprite.setTextureRect(sf::IntRect(
            { currentFrame * frameWidth,  // Shift X position
            frameHeight + state * frameHeight * 2 },      // Y stays at 0 (if horizontal sprite sheet)
            { frameWidth,
            frameHeight }
        ));
    }
}

void Animation::switchState(int _state) {
    state = _state;
    frameTimer = 0.f;
}

void Animation::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void Animation::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Animation::resetFrame() {
    currentFrame = 0;
    frameTimer = 0.f;
}

*/