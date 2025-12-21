#include "Entity.hpp"


Entity::Entity(float _maxHealth, bool destructable, bool alive)
    : maxHealth(_maxHealth), health(_maxHealth), isAlive(alive), isDestructable(destructable), isDying(false),
    entityID(0), typeID(0), faction(0),
    fromCollision(0.f), invincibility(0.2f) 
{}

Entity::Entity(float _maxHealth, float _health, bool destructable, bool alive)
    : maxHealth(_maxHealth), health(_health <= 0 ? 0.f : _health), isAlive(alive), isDestructable(destructable), isDying(false),
    entityID(0), typeID(0), faction(0),
    fromCollision(0.f), invincibility(0.2f)
{}

void Entity::initializeHitbox() {}

void Entity::onCollision(float damage, sf::Vector2f damageOrigin) {
    std::cout << "base entity collision" << std::endl;
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

