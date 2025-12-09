#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Hitbox.hpp"
#include "Entity.hpp"
#include "AnimationController.hpp"


class Player : public Entity {
private:
    // set ptr to self for collision check
    std::weak_ptr<Player> selfPtr;
    sf::CircleShape shape;

    sf::CircleShape center;

    sf::Texture spriteSheet;
    sf::Sprite sprite;
    AnimationController animations;

    float scale = 3.f;
    bool facingLeft = false;
    float spriteOffset = -25.f;


    AnimationController::State targetState = AnimationController::State::IdleRight;

    std::shared_ptr<Hitbox> playerHB;

    float currentStamina;
    float maxStamina;

    float initialSpeed;
    float speed;
    bool keyboardInput = true;

    std::vector<std::shared_ptr<TriggerHitbox>> activeAttacks;
    
    sf::Angle facing;
    sf::Vector2f movementVector;
    // counter for deltaTime -> will print facing angle every 1 second
    float tempTime = 0.f;

    float attackSpeed;
    float attackRange;
    float attackDamage;

    bool attackOnCD = false;
    bool attackHeld = false;
    // bool jump = false;
    bool dashing = false;
    bool dashHeld = false;

public:
    Player();
    ~Player();
    static std::shared_ptr<Player> create();
    void initializeHitbox() override;
    void initializePtr(std::shared_ptr<Player>);

    void handleInput();
    void update(float deltaTime) override;
    void onCollision(float damage) override;
    void render(sf::RenderWindow& window) override;
    float getDirection() override;
    float getSize() override;

    void onAttack();
    sf::Vector2f getPosition() const;
    sf::CircleShape getShape() const;
    // return 2 floats (vector2f) of current/max hp
    sf::Vector2f getHealth() const;
    // return 2 floats (vector2f) of current/max stamina
    sf::Vector2f getStamina() const;

    void updateAnimationState(sf::Vector2f facing);

    void playFootstep();

};
