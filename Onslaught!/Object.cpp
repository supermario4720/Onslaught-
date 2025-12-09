//for obstacles like wood, stones, etc

#include "Object.hpp"
#include "CollisionManager.hpp"

Object::Object(sf::Vector2f position, sf::Vector2f size)
    : Entity(100.f)
{
    // setting corresponding IDs
    setTypeID(3);
    setFaction(2);

    objectShape.setSize(size);
    objectShape.setOrigin(size/2.f);
    objectShape.setPosition(position);
    objectShape.setFillColor(sf::Color::Green);

}
Object::~Object() {
    CollisionManager::getInstance().removeEntityHitbox(objectHB);
}

std::shared_ptr<Object> Object::create(sf::Vector2f position, sf::Vector2f size) {
    std::shared_ptr<Object> obj = std::make_shared<Object>(position, size);
    obj->initializePtr(obj);
    obj->initializeHitbox();
    obj->changeDestructability(false);
    return obj;
}
// create hitbox for player
void Object::initializeHitbox() {
    objectHB = std::make_shared<Hitbox>(selfPtr, objectShape.getPosition(), objectShape.getSize(), 2);
    CollisionManager::getInstance().addEntityHitbox(objectHB);
}

void Object::initializePtr(std::shared_ptr<Object> ptr) {
    selfPtr = ptr;
}

void Object::update(float dt) {
    //std::cout << "calling object update" << std::endl;
    if (fromCollision < 1.0f) fromCollision += dt;
    if (fromCollision > invincibility) objectShape.setFillColor(sf::Color::Green);
}

void Object::onCollision(float damage) {
    //std::cout << "Object collision" << std::endl;
    if (isDestructable && fromCollision > invincibility) {
        objectShape.setFillColor(sf::Color::White);
        if (health > damage) { health -= damage; }
        else {
            health = 0;
            isAlive = false;
        }
    }
    fromCollision = 0.f;
}

void Object::render(sf::RenderWindow& window) {
    window.draw(objectShape);
}

sf::RectangleShape Object::getShape() const {
    return objectShape;
}


// -------- Circle Object ---------

ObjectCirc::ObjectCirc(sf::Vector2f position, float rad)
    : Entity(100.f, false)
{
    objectShape.setRadius(rad);
    objectShape.setOrigin({rad, rad});
    objectShape.setPosition(position);
    objectShape.setFillColor(sf::Color::Green);

}
ObjectCirc::~ObjectCirc() {
    CollisionManager::getInstance().removeEntityHitbox(objectHB);
}

std::shared_ptr<ObjectCirc> ObjectCirc::create(sf::Vector2f position, float rad) {
    std::shared_ptr<ObjectCirc> obj = std::make_shared<ObjectCirc>(position, rad);
    obj->initializePtr(obj);
    obj->initializeHitbox();
    obj->changeDestructability(false);
    return obj;
}
// create hitbox for player
void ObjectCirc::initializeHitbox() {
    objectHB = std::make_shared<Hitbox>(selfPtr, objectShape.getPosition(), objectShape.getRadius());
    CollisionManager::getInstance().addEntityHitbox(objectHB);
}

void ObjectCirc::initializePtr(std::shared_ptr<ObjectCirc> ptr) {
    selfPtr = ptr;
}

void ObjectCirc::update(float dt) {
    //std::cout << "calling object update" << std::endl;
    if (fromCollision < 1.0f) fromCollision += dt;
    if (fromCollision > invincibility) objectShape.setFillColor(sf::Color::Green);
}

void ObjectCirc::onCollision(float damage) {
    //std::cout << "Circle object collision" << std::endl;
    if (isDestructable && fromCollision > invincibility) {
        objectShape.setFillColor(sf::Color::White);
        if (health > damage) { health -= damage; }
        else {
            health = 0;
            isAlive = false;
        }
    }
    fromCollision = 0.f;
}

void ObjectCirc::render(sf::RenderWindow& window) {
    window.draw(objectShape);
}

sf::CircleShape ObjectCirc::getShape() const {
    return objectShape;
}
