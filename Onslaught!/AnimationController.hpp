// AnimationController.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "Animation.hpp"

class AnimationController {
public:
    enum class State {
        IdleRight,
        IdleUp,
        IdleDown,
        WalkRight,
        WalkUp,
        WalkDown,
        AttackRight,
        AttackDown,
        AttackUp,
        Hurt,
        KnockbackRight,
        KnockbackUp,
        KnockbackDown,
        Death
    };

    AnimationController() = default;

    // State, Animation& (Texture& spritesheet, imagecount(Vec2u), rowIndex, frameStart, frameEnd, frameSwitchTime), 
    void addAnimation(State state, const Animation& anim) {
        animations[state] = anim;
    }

    void setState(State newState, bool resetIfSame = false, bool _animateToEnd = false) {
        // if animation needs to finish, return (don't change animation state)
        if (animateToEnd) return;
        else {
            animateToEnd = _animateToEnd;
        }

        if (newState == currentState && !resetIfSame)
            return;

        currentState = newState;
        animations[currentState].reset();
    }
    // Always return false, unless an unlooping animation is finished
    bool update(float dt) {
        bool loop = !animateToEnd;
        // always is false, unless loop is false && the loop is done
        bool finished = animations[currentState].update(dt, loop);
        if (finished) animateToEnd = false;

        return finished;
    }

    void applyToSprite(sf::Sprite& sprite) const {
        animations.at(currentState).applyToSprite(sprite);
    }

    void forceAnimationEnd() {
        animateToEnd = false;
    }

    State getState() {
        return currentState;
    }

private:
    std::unordered_map<State, Animation> animations;
    State currentState = State::IdleRight;

    bool animateToEnd = false;
};
