#include <SFML/Graphics.hpp>
#include "Enemy.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"
#include "BuildingManager.hpp"
#include "Building.hpp"
#include "AudioManager.hpp"
#include "TextureManager.hpp"
#include <iostream>


Enemy::Enemy(sf::Vector2f initPos, float enemySize, float speed)
    :Entity( TextureManager::getInstance().getTexture( "Enemy" ), 100.f), size(enemySize), 
    movementSpeed(speed), entityManager(EntityManager::getInstance()), detectionRadius(150.f)
{
    // setting corresponding IDs
    setTypeID(2);
    setFaction(1);    

    enemy.setRadius(size);
    enemy.setOrigin({size, size});
    enemy.setPosition(initPos);

    movementVector = { 0.f, 0.f };
    facing = 0.f;
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
    entityHitbox = std::make_shared<Hitbox>(selfPtr, getPosition(), size, 1);
    CollisionManager::getInstance().addEntityHitbox(entityHitbox);
}

void Enemy::initializePtr(std::shared_ptr<Enemy> ptr) {
    selfPtr = ptr;
}


void Enemy::update(float dt) {
}

void Enemy::update(float dt, const BuildingManager& buildManager) {
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
    sf::Vector2f targetPos = chooseTarget(buildManager);
    movementVector = targetPos - getPosition();

    updatePosition(movementVector, dt);
    if (!isTakingDamage) updateAnimationState(movementVector);

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

    if (fromCollision < 1.0f) fromCollision += dt;
    if (fromCollision > invincibility) {
        isTakingDamage = false;
    }
    
    if (currentKnockbackTime < knockbackDuration) {
        currentKnockbackTime += dt;
    }
    else {
        knockbackVector = sf::Vector2f ( {0.f, 0.f} );
    }
    //std::cout << fromCollision << std::endl;
}

void Enemy::updatePosition(sf::Vector2f movementVec, float dt) {
    sf::Vector2f fixedMovementVec = movementVec;
    // turn to unit vector, then multiply by speed
    if (fixedMovementVec.x != 0.f || fixedMovementVec.y != 0.f) {
        fixedMovementVec = fixedMovementVec.normalized();
        facing = fixedMovementVec.angle().wrapUnsigned().asDegrees();

        fixedMovementVec = fixedMovementVec + knockbackVector;
        fixedMovementVec *= movementSpeed * dt;

        sf::Vector2f collidedVec = entityHitbox->updateHitbox(fixedMovementVec, true, attackDamage);
        sprite.move(collidedVec);
    }
    else {
        return;
    }
}

void Enemy::onCollision(float damage, sf::Vector2f damageOrigin) {
    //std::cout << "enemy collide" << std::endl;
    if (fromCollision > invincibility && targetState != AnimationController::State::Death) {
        // 0.1f time for invicibility
        fromCollision = 0.f;
        // when taking damage, calculate knockback
        calculateKnockback(damageOrigin);
        isTakingDamage = true;

        if (health > damage) { 
            isAttacking = false;
            health -= damage;

            AudioManager::getInstance().play("enemyHurt", true, 30.f);

            animations.forceAnimationEnd();
            if (facing < 45.f || facing > 315.f) {
                targetState = AnimationController::State::KnockbackRight;
            }
            if (facing <= 135.f && facing >= 45.f) {
                targetState = AnimationController::State::KnockbackUp;
            }
            if (facing < 225.f && facing > 135.f) {
                targetState = AnimationController::State::KnockbackRight;
                facingLeft = true;
            }
            if (facing <= 315.f && facing >= 225.f) {
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

void Enemy::onAttack() {
    if (facing < 45.f || facing > 315.f) {
        targetState = AnimationController::State::AttackRight;
    }
    if (facing <= 135.f && facing >= 45.f) {
        targetState = AnimationController::State::AttackDown;
    }
    if (facing < 225.f && facing > 135.f) {
        targetState = AnimationController::State::AttackRight;
        facingLeft = true;
    }
    if (facing <= 315.f && facing >= 225.f) {
        targetState = AnimationController::State::AttackUp;
    }
    AudioManager::getInstance().play("enemyAttack", false, 30.f);
    isAttacking = true;
}

void Enemy::onDeath() {
    // call EntityManager instance to get item manager, and spawn item
    entityManager.spawnExp(getPosition(), droppedExp);
    isAlive = false;
}

const sf::Vector2f Enemy::getPosition() const {
	return sprite.getPosition();
}

const std::weak_ptr<Hitbox> Enemy::getHitbox() const {
    return entityHitbox;
}

void Enemy::render(sf::RenderWindow& window) {
    //window.draw(enemy);
    window.draw(sprite);
    entityHitbox->render(window);
}

void Enemy::updateAnimationState(sf::Vector2f moveVec) {
    if (targetState == AnimationController::State::Death) return;
    facingLeft = false;

    if (facing < 45.f || facing > 315.f) {
        targetState = AnimationController::State::IdleRight;
    }
    if (facing <= 135.f && facing >= 45.f) {
        targetState = AnimationController::State::IdleDown;
    }
    if (facing < 225.f && facing > 135.f) {
        targetState = AnimationController::State::IdleRight;
        facingLeft = true;
    }
    if (facing <= 315.f && facing >= 225.f) {
        targetState = AnimationController::State::IdleUp;
    }

    if (moveVec.x != 0 || moveVec.y != 0) {
        if (facing < 45.f || facing > 315.f) {
            targetState = AnimationController::State::WalkRight;
        }
        if (facing <= 135.f && facing >= 45.f) {
            targetState = AnimationController::State::WalkDown;
        }
        if (facing < 225.f && facing > 135.f) {
            targetState = AnimationController::State::WalkRight;
            facingLeft = true;
        }
        if (facing <= 315.f && facing >= 225.f) {
            targetState = AnimationController::State::WalkUp;
        }
    }
}

void Enemy::calculateKnockback(sf::Vector2f damageOrigin) {
    currentKnockbackTime = 0.f;
    knockbackVector = sprite.getPosition() - damageOrigin;

    if(knockbackVector.x != 0 || knockbackVector.y != 0) knockbackVector = knockbackVector.normalized()*3.f;
}

sf::Vector2f Enemy::chooseTarget(const BuildingManager& buildManager) {
    sf::Vector2f playerPos = entityManager.getPlayerPos();
    sf::Vector2f thisPos = getPosition();
    sf::Vector2f distVec = playerPos - thisPos;
    float dist = detectionRadius - 1.f;

    if(distVec.x != 0 || distVec.y != 0) {
        dist = distVec.length();
    }
    // if player distance within radius, return player position
    if(dist < detectionRadius) return playerPos;

    // check if town within radius next
    sf::Vector2f townPos = {0.f, 0.f};
    distVec = townPos - thisPos;
    if(distVec.x != 0 || distVec.y != 0) dist = distVec.length();
    if(dist < detectionRadius) {
        return townPos;
    }

    sf::Vector2f buildingPos({0.f, 0.f});
    sf::Vector2f targetPos({0.f, 0.f});
    float buildDist = dist;
    
    const auto& buildings = buildManager.getBuildingList();
    for (auto& building : buildings) {

        buildingPos = building->getPosition();
        distVec = buildingPos - thisPos;

        if(distVec.x != 0 || distVec.y != 0) buildDist = distVec.length();
        if(buildDist < detectionRadius) {
            if(buildDist < dist) dist = buildDist;
            targetPos = buildingPos;
        }
    }
    if(dist < detectionRadius) {
        return targetPos;
    }

    return townPos;
    
}