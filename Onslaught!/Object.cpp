//for obstacles like wood, stones, etc

#include "Object.hpp"
#include "ObjectData.hpp"
#include "ObjectTextures.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"


Object::Object(ObjectID id, sf::Vector2f position)
    : Entity(ObjectDatabase::get(id).maxHealth), objID(id),
    sprite(ObjectTextures::getInstance().getTexture(id))
{
    // setting corresponding IDs
    setTypeID(3);
    setFaction(2);

    const ObjectData& obj = ObjectDatabase::get(id);

    sf::Vector2f bounds = sprite.getGlobalBounds().size;
    sprite.setOrigin(bounds/2.f);
    // if the object is a rect
    if(obj.rad == 0.f) {
        float scaleX = obj.size.x / bounds.x;
        float scaleY = obj.size.y / bounds.y;
        sprite.setScale( {scaleX*1.1f, scaleY*1.1f} );
    }
    else {
        float scaleX = obj.rad*2.f / bounds.x;
        float scaleY = obj.rad*2.f / bounds.y;
        sprite.setScale( {scaleX*1.1f, scaleY*1.1f} );
    }
    spriteOffset = 5.f;
    sprite.setPosition({position.x, position.y + spriteOffset});

}
Object::~Object() {
    CollisionManager::getInstance().removeEntityHitbox(objectHB);
}

std::shared_ptr<Object> Object::create(ObjectID id, sf::Vector2f position) {
    std::shared_ptr<Object> obj = std::make_shared<Object>(id, position);
    obj->initializePtr(obj);
    obj->initializeHitbox();
    return obj;
}
// create hitbox for player
void Object::initializeHitbox() {
    const ObjectData& obj = ObjectDatabase::get(objID);
    if(obj.rad == 0.f) {
        objectHB = std::make_shared<Hitbox>(selfPtr, sprite.getPosition(), obj.size, 2);
        CollisionManager::getInstance().addEntityHitbox(objectHB);
    }
    else{
        objectHB = std::make_shared<Hitbox>(selfPtr, sprite.getPosition(), obj.rad, 2);
        CollisionManager::getInstance().addEntityHitbox(objectHB);
    }
}

void Object::initializePtr(std::shared_ptr<Object> ptr) {
    selfPtr = ptr;
}

void Object::update(float dt) {
    //std::cout << "calling object update" << std::endl;
    if (fromCollision < 1.0f) fromCollision += dt;
    if (fromCollision > invincibility) sprite.setColor(sf::Color(255,255,255,255));
}

void Object::onCollision(float damage, sf::Vector2f damageOrigin) {
    //std::cout << "Object collision" << std::endl;
    if (isDestructable && fromCollision > invincibility) {
        sprite.setColor(sf::Color(255,255,255,125));
        if (health > damage) { health -= damage; }
        else {
            health = 0;
            onDeath();
        }
    }
    fromCollision = 0.f;
}

void Object::onDeath() {
    const ObjectData& obj = ObjectDatabase::get(objID);
    int dropQty = randInt(obj.minDropQty, obj.maxDropQty);
    EntityManager::getInstance().spawnItems(obj.dropItem, sprite.getPosition(), dropQty);
    
    isAlive = false;
}

void Object::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

const sf::Vector2f Object::getPosition() const {
    return sprite.getPosition();
}