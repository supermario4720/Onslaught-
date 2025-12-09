#include "Player.hpp"
#include "Hitbox.hpp"
#include "CollisionManager.hpp"
#include "AudioManager.hpp"
#include <iostream>

Player::Player()
    : Entity(100.f), initialSpeed(80.f), attackSpeed(550.f), attackRange(30.f), maxStamina(100.f), currentStamina(100.f),
    spriteSheet("resources/dwarfx1.png"), sprite(spriteSheet)
{
    // setting corresponding IDs
    setTypeID(1);
    setFaction(0);

    float rad = 24.f;
    sf::Vector2f pos = { 0.f, 100.f };

    shape.setRadius(rad);
    shape.setOrigin({ rad, rad });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Blue);
    speed = initialSpeed;

    center.setRadius(2);
    center.setOrigin({ 1,1 });
    center.setPosition(pos);
    center.setFillColor(sf::Color::White);


    movementVector = { 0.f, 0.f };
    facing = sf::degrees(0);

    changeInvincibility(0.5f);
    attackDamage = 20.f;

    sf::Vector2u imageCount(5, 9);
    // find the size of each sprite
    sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
    spriteSize.x /= (float)imageCount.x;
    spriteSize.y /= (float)imageCount.y;

    sprite.setOrigin(spriteSize / 2.f);
    sprite.setScale({ scale, scale });
    sprite.setPosition({pos.x, pos.y + spriteOffset});

    animations.addAnimation(AnimationController::State::IdleRight,
        Animation(spriteSheet, imageCount, 0, 0, 0, 1.0f));
    animations.addAnimation(AnimationController::State::IdleUp,
        Animation(spriteSheet, imageCount, 0, 1, 1, 1.0f));
    animations.addAnimation(AnimationController::State::IdleDown,
        Animation(spriteSheet, imageCount, 0, 2, 2, 1.0f));

    animations.addAnimation(AnimationController::State::WalkRight,
        Animation(spriteSheet, imageCount, 1, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::WalkUp,
        Animation(spriteSheet, imageCount, 2, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::WalkDown,
        Animation(spriteSheet, imageCount, 3, 0, 3, 0.10f));

    animations.addAnimation(AnimationController::State::AttackRight,
        Animation(spriteSheet, imageCount, 4, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::AttackDown,
        Animation(spriteSheet, imageCount, 5, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::AttackUp,
        Animation(spriteSheet, imageCount, 6, 0, 3, 0.10f));

    animations.addAnimation(AnimationController::State::Hurt,
        Animation(spriteSheet, imageCount, 7, 0, 0, 0.15f));
    animations.addAnimation(AnimationController::State::Death,
        Animation(spriteSheet, imageCount, 8, 0, 3, 0.15f));

}

Player::~Player() {
    activeAttacks.clear();
}

// create player and return pointer (so hitbox not initialized in constructor)
std::shared_ptr<Player> Player::create() {
    std::shared_ptr<Player> player = std::shared_ptr<Player>(new Player());
    player->initializePtr(player);
    player->initializeHitbox();
    return player;
}
// create hitbox for player
void Player::initializeHitbox() {
    playerHB = std::make_shared<Hitbox>( selfPtr, shape.getPosition(), shape.getRadius(), 0);
    playerHB->changeVisibility(false);
    CollisionManager::getInstance().addEntityHitbox(playerHB);
}

void Player::initializePtr(std::shared_ptr<Player> ptr) {
    selfPtr = ptr;
}


// Handle input by using booleans -> for each button used, check if clicked, then turn respective boolean to true
void Player::handleInput() {
    movementVector = { 0.f, 0.f };
    // for when using keyboard control scheme
    if (keyboardInput) {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))  movementVector.x -= 1.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))  movementVector.x += 1.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))  movementVector.y -= 1.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))  movementVector.y += 1.f;

        updateAnimationState(movementVector);

        // check if button being held -> prevents attack from spamming when button held
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) ) {
            if (!attackOnCD && !attackHeld && currentStamina > 10.f) {
                onAttack();
                currentStamina -= 10.f;
            }
            attackHeld = true;
        }
        else { attackHeld = false;  }

        // sprint -> if button pressed, make player move 1.5 times faster
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
            if (currentStamina > 0.3f) {
                speed = initialSpeed * 1.5f;
                currentStamina -= 0.3f;
                dashing = true;
            }
            else {
                speed = initialSpeed;
                dashing = false;
            }
            dashHeld = true;
        }
        else { 
            dashHeld = false; 
            dashing = false;
            speed = initialSpeed;
        }
    }
}

void Player::update(float deltaTime) {
    if (!isAlive) {
        std::cout << isAlive << std::endl;
        return;
    }
    AudioManager& audio = AudioManager::getInstance();

    if (targetState == AnimationController::State::Death) {
        animations.forceAnimationEnd();
        animations.setState(targetState, false, !(targetState != AnimationController::State::AttackRight &&
            targetState != AnimationController::State::AttackDown &&
            targetState != AnimationController::State::AttackUp &&
            targetState != AnimationController::State::Hurt &&
            targetState != AnimationController::State::Death));
        // true until animation is over
        isAlive = !animations.update(deltaTime);
        animations.applyToSprite(sprite);
        return;
    }

    // update inputs
    handleInput();
    //std::cout << movementVector.x << ", " << movementVector.y << std::endl;

    // Normalize vector if non zero and multiply by movement speed
    if (movementVector.x != 0.f || movementVector.y != 0.f) {
        
        movementVector = movementVector.normalized() * speed;
    }

    movementVector *= deltaTime;
    
    if (movementVector.x != 0.f || movementVector.y != 0.f) {
        audio.play("playerWalkFootstep", false, 20.f);
        facing = movementVector.angle().wrapSigned();
    }

    // apply changes virtually to check for collision
    sf::CircleShape movedShapeX = shape;
    sf::CircleShape movedShapeY = shape;

    movedShapeX.move( { movementVector.x, 0.f} );
    movedShapeY.move( { 0.f, movementVector.y} );


    std::vector<std::weak_ptr<Hitbox>>& entityHitboxes = CollisionManager::getInstance().getEntityHitboxList();
    for (auto entityIteration = entityHitboxes.begin(); entityIteration != entityHitboxes.end();) {
        if (entityIteration->expired()) {
            entityIteration++;
            continue;
        }
        auto hitbox = entityIteration->lock();
        // skip own hitbox
        if (hitbox.get() == playerHB.get()) {
            entityIteration++;
            continue;
        }
        // if hitbox type is rectangle
        if (hitbox->getType() == 0) {
            const sf::RectangleShape& rect = hitbox -> getRectHitbox();
            if ( CollisionManager::checkCircleRectCollision(movedShapeX, rect ) ) {
                // if collision on x axis, make x movement 0
                movementVector.x = 0.f;
            }
            if (CollisionManager::checkCircleRectCollision(movedShapeY, rect) ) {
                // if collision on y axis, make y movement 0
                movementVector.y = 0.f;
            }
        }
        // if hitbox type is circle 
        else if (hitbox-> getType() == 1) {
            const sf::CircleShape& circle = hitbox-> getCircleHitbox();
            /*
            if ( CollisionManager::checkCircleCollision(movedShapeX, circle ) ) {
                movementVector.x = 0.f;
            }
            if (CollisionManager::checkCircleCollision(movedShapeY, circle) ) {
                movementVector.y = 0.f;
            }
            */
            movementVector = CollisionManager::slideAgainstCircle(shape, circle, movementVector);
        }
        entityIteration++;
    }
    CollisionManager::getInstance().removeExpiredHitboxes();

    /*
    IF colliding with rect on corner, should not jsut get stuck, but slide outwards 
    */

    animations.setState(targetState, false, !(targetState != AnimationController::State::AttackRight &&
        targetState != AnimationController::State::AttackDown &&
        targetState != AnimationController::State::AttackUp &&
        targetState != AnimationController::State::Hurt &&
        targetState != AnimationController::State::Death) );

    // if attack is on cooldown and not in attack animation, reset cooldown
    if (attackOnCD) {
        AnimationController::State currentState = animations.getState();

        if (currentState != AnimationController::State::AttackRight &&
            currentState != AnimationController::State::AttackDown &&
            currentState != AnimationController::State::AttackUp)
        {
            attackOnCD = false;
        }
    }

    animations.update(deltaTime);
    animations.applyToSprite(sprite);
    // change left or right direction
    if (facingLeft) {
        sprite.setScale({ -scale, scale });
    }
    else {
        sprite.setScale({ scale, scale });
    }

    shape.move(movementVector);
    sf::Vector2f newPos = shape.getPosition();
    playerHB->updateHitbox(newPos);
    sprite.setPosition({ newPos.x, newPos.y + spriteOffset });
    
    center.setPosition(getPosition());

    // sprite.setRotation(facing + sf::degrees(90));

    // update attack boxes
    for (auto attack = activeAttacks.begin(); attack != activeAttacks.end(); ) {
        // it is only pointer, must get value of ptr with (*ptr)
        (*attack)->updateTriggerHitbox(deltaTime, getPosition(), facing);

        if ((*attack)->isExpired()) {
            attack = activeAttacks.erase(attack);
            // reset attack cooldown if attack is done
        }
        else {
            ++attack;
        }
    }
    // modify collision timer, collision effect
    if (fromCollision < 3.0f) fromCollision += deltaTime;
    else {
        //replace 0.05f with regen speed
        if (health + 0.02f > maxHealth) health = maxHealth;
        else health += 0.02f;
    }
    if (fromCollision > 0.1f) {
        sprite.setColor(sf::Color(255, 255, 255, 255));
        shape.setFillColor(sf::Color::Blue);
    }
    // recover stamina if not dashing or holding attack
    if (!dashHeld && !attackHeld) {
        if (currentStamina + 1.f > maxStamina) currentStamina = maxStamina;
        else currentStamina += 0.2f;
    }

}

void Player::onCollision(float damage) {
    // if not in death animation and is not invulnerable time
    if (fromCollision > invincibility && targetState != AnimationController::State::Death) {
        std::cout << "Player collision" << std::endl;
        shape.setFillColor(sf::Color::White);
        fromCollision = 0.f;
        targetState = AnimationController::State::Hurt;
        sprite.setColor(sf::Color(255, 255, 255, 125));
        if (health > damage) { 
            health -= damage; 
            AudioManager::getInstance().play("playerHurt", false, 40.f);
        }
        else {
            health = 0;
            // isAlive = false;
            // dont turn dead instantly, finish death animation first
            targetState = AnimationController::State::Death;
            AudioManager::getInstance().play("playerDeath", false, 80.f);
        }
    }
}

void Player::render(sf::RenderWindow& window) {
    // window.draw(shape);
    window.draw(sprite);
    // window.draw(center);
    for (auto& hb : activeAttacks) hb -> render(window);
}

float Player::getDirection() {
    return facing.asDegrees();
}

float Player::getSize() {
    return shape.getRadius();
}

void Player::onAttack() {
    sf::Vector2f position = getPosition();
    auto newAttack = std::make_shared<SwingHitbox>(selfPtr, position, sf::Vector2f{ attackRange, 10.f }, attackDamage, attackSpeed, 150.f, 0);
    //newAttack->changeVisibility(true);
    activeAttacks.push_back(newAttack);
    attackOnCD = true;

    float degrees = facing.wrapUnsigned().asDegrees();
    if (degrees < 45.f || degrees > 315.f) {
        targetState = AnimationController::State::AttackRight;
    }
    if (degrees <= 135.f && degrees >= 45.f) {
        targetState = AnimationController::State::AttackDown;
    }
    if (degrees < 225.f && degrees > 135.f) {
        targetState = AnimationController::State::AttackRight;
        facingLeft = true;
    }
    if (degrees <= 315.f && degrees >= 225.f) {
        targetState = AnimationController::State::AttackUp;
    }

    AudioManager::getInstance().play("playerAttack", true, 80.f);
}

sf::CircleShape Player::getShape() const {
    return shape;
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Player::getHealth() const {
    return { health , maxHealth };
}

sf::Vector2f Player::getStamina() const {
    return { currentStamina , maxStamina };
}

void Player::updateAnimationState(sf::Vector2f moveVec) {
    if (targetState == AnimationController::State::Death) return;
    facingLeft = false;

    float degrees = facing.wrapUnsigned().asDegrees();
    if (degrees < 45.f || degrees > 315.f) {
        targetState = AnimationController::State::IdleRight;
    }
    if (degrees <= 135.f && degrees >= 45.f) {
        targetState = AnimationController::State::IdleDown;
    }
    if (degrees < 225.f && degrees > 135.f) {
        targetState = AnimationController::State::IdleRight;
        facingLeft = true;
    }
    if (degrees <= 315.f && degrees >= 225.f) {
        targetState = AnimationController::State::IdleUp;
    }


    if (moveVec.x > 0) {
        targetState = AnimationController::State::WalkRight;
    }
    else if (moveVec.x < 0) {
        targetState = AnimationController::State::WalkRight;
        facingLeft = true;
    }
    if (moveVec.y > 0) {
        targetState = AnimationController::State::WalkDown;
    }
    else if (moveVec.y < 0) {
        targetState = AnimationController::State::WalkUp;
    }
}

void Player::playFootstep() {
    AudioManager& aud = AudioManager::getInstance();

    sf::Sound footstepSound(aud.getBuffer("playerWalkFootstep"));
    if (footstepSound.getStatus() != sf::Sound::Status::Playing) {
        aud.play("playerWalkFootstep");
    }
}