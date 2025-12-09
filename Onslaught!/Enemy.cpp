#include <SFML/Graphics.hpp>
#include "Enemy.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"
#include "AudioManager.hpp"
#include <iostream>


Enemy::Enemy(sf::Vector2f initPos, float enemySize, float speed)
    :Entity(100.f), size(enemySize), movementSpeed(speed), entityManager(EntityManager::getInstance()),
    spriteSheet("resources/skeleton.png"), sprite(spriteSheet)
{
    // setting corresponding IDs
    setTypeID(2);
    setFaction(1);

    //movementSpeed = 10.f;
    

    enemy.setRadius(size);
    enemy.setOrigin({ size, size });
    enemy.setPosition(initPos);
    enemy.setFillColor(sf::Color::Red);

    movementVector = { 0.f, 0.f };
    facing = sf::degrees(0);
    knockbackVector = { 0.f, 0.f };
    knockbackDuration = 0.2f;
    currentKnockbackTime = 0.f;
    isAttacking = false;
    isTakingDamage = false;
    attackDamage = 10.f;

    changeInvincibility(0.3f);

    sf::Vector2u imageCount(4, 12);
    // find the size of each sprite
    sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
    spriteSize.x /= (float)imageCount.x;
    spriteSize.y /= (float)imageCount.y;
    
    sprite.setOrigin(spriteSize / 2.f);
    sprite.setScale({ scale, scale });
    sprite.setPosition({ initPos.x, initPos.y + spriteOffset });

    animations.addAnimation(AnimationController::State::IdleDown,
        Animation(spriteSheet, imageCount, 0, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::IdleRight,
        Animation(spriteSheet, imageCount, 1, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::IdleUp,
        Animation(spriteSheet, imageCount, 2, 0, 3, 0.10f));

    animations.addAnimation(AnimationController::State::WalkDown,
        Animation(spriteSheet, imageCount, 3, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::WalkRight,
        Animation(spriteSheet, imageCount, 4, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::WalkUp,
        Animation(spriteSheet, imageCount, 5, 0, 3, 0.10f));

    animations.addAnimation(AnimationController::State::AttackDown,
        Animation(spriteSheet, imageCount, 3, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::AttackRight,
        Animation(spriteSheet, imageCount, 4, 0, 3, 0.10f));
    animations.addAnimation(AnimationController::State::AttackUp,
        Animation(spriteSheet, imageCount, 5, 0, 3, 0.10f));

    animations.addAnimation(AnimationController::State::KnockbackUp,
        Animation(spriteSheet, imageCount, 6, 0, 1, 0.10f));
    animations.addAnimation(AnimationController::State::KnockbackRight,
        Animation(spriteSheet, imageCount, 7, 0, 1, 0.10f));
    animations.addAnimation(AnimationController::State::KnockbackDown,
        Animation(spriteSheet, imageCount, 8, 0, 1, 0.10f));

    animations.addAnimation(AnimationController::State::Death,
        Animation(spriteSheet, imageCount, 9, 0, 3, 0.10f));
}

// create player and return pointer (so hitbox not initialized in constructor)
std::shared_ptr<Enemy> Enemy::create(sf::Vector2f initPos, float enemySize, float speed) {
    std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(initPos, enemySize, speed);
    enemy->initializePtr(enemy);
    enemy->initializeHitbox();
    return enemy;
}
Enemy::~Enemy() {
}

// create hitbox for player
void Enemy::initializeHitbox() {
    enemyHB = std::make_shared<Hitbox>(selfPtr, enemy.getPosition(), enemy.getRadius(), 1);
    CollisionManager::getInstance().addEntityHitbox(enemyHB);
}

void Enemy::initializePtr(std::shared_ptr<Enemy> ptr) {
    selfPtr = ptr;
}


void Enemy::update(float dt) {
    if (!isAlive) {
        return;
    }
    if (isDying) {
        onDeath();
        return;
    }

    AudioManager& audio = AudioManager::getInstance();

    if (targetState == AnimationController::State::Death) {
        animations.forceAnimationEnd();
        animations.setState(targetState, false, !(targetState != AnimationController::State::Death));
        // true when animation is over
        isDying = animations.update(dt);
        animations.applyToSprite(sprite);
        return;
    }
 
    // check player position from manager instance
    sf::Vector2f playerPos = entityManager.getPlayerPos();
    movementVector = playerPos - getPosition();

    // turn to unit vector, then multiply by speed
    if (movementVector.x != 0.f || movementVector.y != 0.f) {
        movementVector = movementVector.normalized();
        facing = movementVector.angle().wrapSigned();
        movementVector *= movementSpeed * dt;
        //audio.play("enemyFootstep", false, 50.f);
    }

    sf::CircleShape movedShapeX = enemy;
    sf::CircleShape movedShapeY = enemy;

    sf::Vector2f tempMoveVec = movementVector;

    if (!isTakingDamage) updateAnimationState(tempMoveVec);

    const std::vector<std::weak_ptr<Hitbox>>& entityHitboxes = CollisionManager::getInstance().getEntityHitboxList();
    for (const auto& hitbox : entityHitboxes) {
        bool collided = false;
        // skip own or expired hitbox
        if (hitbox.expired()) {
            continue;
        }
        auto hb = hitbox.lock();
        if (hb.get() == enemyHB.get()) {
            continue;
        }

        movedShapeX = enemy;
        movedShapeY = enemy;
        movedShapeX.move({ tempMoveVec.x, 0.f });
        movedShapeY.move({ 0.f, tempMoveVec.y });

        // if hitbox type is rectangle
        if (hb->getType() == 0) {
            const sf::RectangleShape& rect = hb->getRectHitbox();
            if (CollisionManager::checkCircleRectCollision(movedShapeX, rect)) {
                // if collision on x axis, make x movement 0
                tempMoveVec.x = 0.f;
                collided = true;
            }
            if (CollisionManager::checkCircleRectCollision(movedShapeY, rect)) {
                // if collision on y axis, make y movement 0
                tempMoveVec.y = 0.f;
                collided = true;
            }
        }
        // if hitbox type is circle 
        else if (hb->getType() == 1) {
            const sf::CircleShape& circle = hb->getCircleHitbox();
            
            if (CollisionManager::checkCircleCollision(movedShapeX, circle) || CollisionManager::checkCircleCollision(movedShapeY, circle)) {
                tempMoveVec = CollisionManager::slideAgainstCircle(enemy, circle, tempMoveVec);
                collided = true;
            }
        }

        if (collided && !isAttacking && !isTakingDamage) {
            if (hb->getFaction() != faction) {
                hb->onCollision(attackDamage);
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
                audio.play("enemyAttack", false, 30.f);
            }
            isAttacking = true;
        }
    }
    CollisionManager::getInstance().removeExpiredHitboxes();


    animations.setState(targetState, false, !(targetState != AnimationController::State::AttackRight &&
        targetState != AnimationController::State::AttackDown &&
        targetState != AnimationController::State::AttackUp &&
        targetState != AnimationController::State::KnockbackRight &&
        targetState != AnimationController::State::KnockbackUp &&
        targetState != AnimationController::State::KnockbackDown &&
        targetState != AnimationController::State::Death));

    // if attacking, check until animation finishes
    if (isAttacking) {
        isAttacking = !animations.update(dt);
        // if attack is finished, set state to reset cycle
        if (!isAttacking) animations.setState(AnimationController::State::IdleRight);
    }
    else if (isTakingDamage) {
        isTakingDamage = !animations.update(dt);
        if (!isAttacking) animations.setState(AnimationController::State::IdleRight);
    }
    else {
        animations.update(dt);
    }

    animations.applyToSprite(sprite);
    // change left or right direction
    if (facingLeft) {
        sprite.setScale({ -scale, scale });
    }
    else {
        sprite.setScale({ scale, scale });
    }

    enemy.move(tempMoveVec + knockbackVector*dt);

    sf::Vector2f newPos = enemy.getPosition();
    enemyHB->updateHitbox(newPos);
    sprite.setPosition({ newPos.x, newPos.y + spriteOffset });

    if (fromCollision < 1.0f) fromCollision += dt;
    if (fromCollision > invincibility) {
        enemy.setFillColor(sf::Color::Red);
        isTakingDamage = false;
    }
    
    if (currentKnockbackTime < knockbackDuration) {
        currentKnockbackTime += dt;
    }
    else {
        knockbackVector = sf::Vector2f ( { 0.f, 0.f } );
    }
    //std::cout << fromCollision << std::endl;
}

void Enemy::onCollision(float damage) {
    //std::cout << "enemy collide" << std::endl;
    if (fromCollision > invincibility && targetState != AnimationController::State::Death) {
        // 0.1f time for invicibility
        enemy.setFillColor(sf::Color::White);
        fromCollision = 0.f;
        // when taking damage, calculate knockback
        calculateKnockback();
        isTakingDamage = true;

        if (health > damage) { 
            isAttacking = false;

            float degrees = facing.wrapUnsigned().asDegrees();
            health -= damage;

            AudioManager::getInstance().play("enemyHurt", true, 30.f);

            animations.forceAnimationEnd();
            if (degrees < 45.f || degrees > 315.f) {
                targetState = AnimationController::State::KnockbackRight;
            }
            if (degrees <= 135.f && degrees >= 45.f) {
                targetState = AnimationController::State::KnockbackUp;
            }
            if (degrees < 225.f && degrees > 135.f) {
                targetState = AnimationController::State::KnockbackRight;
                facingLeft = true;
            }
            if (degrees <= 315.f && degrees >= 225.f) {
                targetState = AnimationController::State::KnockbackDown;
            }
        }
        else {
            health = 0;
            AudioManager::getInstance().play("enemyDeath", true, 30.f);
            targetState = AnimationController::State::Death;
        }
    }
}

void Enemy::onDeath() {
    // call EntityManager instance to get item manager, and spawn item
    entityManager.spawnItems(drop, getPosition(), qty);

    isAlive = false;
}

sf::Vector2f Enemy::getPosition() {
	return enemy.getPosition();
}

void Enemy::render(sf::RenderWindow& window) {
    //window.draw(enemy);
    window.draw(sprite);
}

void Enemy::updateAnimationState(sf::Vector2f moveVec) {
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

    if (moveVec.x != 0 || moveVec.y != 0) {
        if (degrees < 45.f || degrees > 315.f) {
            targetState = AnimationController::State::WalkRight;
        }
        if (degrees <= 135.f && degrees >= 45.f) {
            targetState = AnimationController::State::WalkDown;
        }
        if (degrees < 225.f && degrees > 135.f) {
            targetState = AnimationController::State::WalkRight;
            facingLeft = true;
        }
        if (degrees <= 315.f && degrees >= 225.f) {
            targetState = AnimationController::State::WalkUp;
        }
    }
}

void Enemy::calculateKnockback() {
    currentKnockbackTime = 0.f;
    float facingAngle = facing.asRadians();

    sf::Vector2f facingVec( { std::cos(facingAngle), std::sin(facingAngle) } );
    knockbackVector = -facingVec * movementSpeed * 3.f;

}