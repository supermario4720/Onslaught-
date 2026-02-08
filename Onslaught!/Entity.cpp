#include "Entity.hpp"
#include "Hitbox.hpp"
#include "CollisionManager.hpp"


Entity::Entity(const sf::Texture& tex, float _maxHealth, bool destructable, bool alive)
    : maxHealth(_maxHealth), health(_maxHealth), isAlive(alive), isDestructable(destructable), isDying(false),
    entityID(0), typeID(0), faction(0),
    fromCollision(0.f), invincibility(0.2f),
    spriteSheet(tex), sprite(tex)
{}

Entity::Entity(const sf::Texture& tex, float _maxHealth, float _health, bool destructable, bool alive)
    : maxHealth(_maxHealth), health(_health <= 0 ? 0.f : _health), isAlive(alive), isDestructable(destructable), isDying(false),
    entityID(0), typeID(0), faction(0),
    fromCollision(0.f), invincibility(0.2f),
    spriteSheet(tex), sprite(tex)
{}

void Entity::initializeHitbox() {}

void Entity::onCollision(float damage, sf::Vector2f damageOrigin) {
    std::cout << "base entity collision" << std::endl;
}

void Entity::onAttack() {
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
}


void Entity::update(float dt) {
    std::cout << "calling base entity update" << std::endl;
}

float Entity::getSize() { 
    return 30.f;
}

const sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::updatePosition(sf::Vector2f movementVec, float dt) {
    sf::Vector2f fixedMovementVec = movementVec;
     // turn to unit vector, then multiply by speed
    if (fixedMovementVec.x != 0.f || fixedMovementVec.y != 0.f) {
        fixedMovementVec = fixedMovementVec.normalized();
        facing = fixedMovementVec.angle().wrapUnsigned().asDegrees();

        fixedMovementVec = fixedMovementVec + knockbackVector;
        fixedMovementVec *= movementSpeed*dt;
        //audio.play("enemyFootstep", false, 50.f);


        sf::Vector2f collidedVec = entityHitbox->updateHitbox(fixedMovementVec);
        sprite.move(collidedVec);
    }
    else {
        return;
    }
}

float Entity::getDirection() { 
    return 0.f; 
}

void Entity::render(sf::RenderWindow& window) {}

bool Entity::checkAlive() {
    return isAlive;
}

void Entity::onDeath() {};

void Entity::changeDestructability(bool dest) {
    isDestructable = dest;
}

void Entity::changeInvincibility(float invincTime) {
    invincibility = invincTime;
}

void Entity::setMaxHealth(float maxhp) {
    maxHealth = maxhp;
    health = maxhp;
}

// get ID of entity instance
int Entity::getEntityID() const {
    return entityID;
}
// set entity instance ID
void Entity::setEntityID(int id) {
    entityID = id;
}
// get entity type ID
int Entity::getTypeID() const {
    return typeID;
}
// set entity type ID
void Entity::setTypeID(int id) {
    typeID = id;
}
// get faction ID
int Entity::getFaction() const {
    return faction;
}
// set faction ID
void Entity::setFaction(int _faction) {
    faction = _faction;
}

