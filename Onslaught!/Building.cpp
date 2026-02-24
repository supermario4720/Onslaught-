#include "Building.hpp"
#include "CollisionManager.hpp"
#include "AudioManager.hpp"
#include <SFML/Graphics.hpp>


Building::Building(BuildingID id, sf::Vector2f pos, int _faction)
: Entity( TextureManager::getInstance().getTexture( id ), BuildingDatabase::get(id).health ),
buildingID(id), expired(false)
{
	position = pos;
	faction = _faction;

	sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
	sf::Vector2f buildingSize = BuildingDatabase::get(id).size;
    sprite.setOrigin({spriteSize.x / 2.f, spriteSize.y/2.f});
	float scaleX = (buildingSize.x / spriteSize.x);
	float scaleY = (buildingSize.y / spriteSize.y);
	sprite.setScale({scaleX * 1.15f, scaleY * 1.15f});
	spriteOffset = 7.f;
    sprite.setPosition({pos.x, pos.y + spriteOffset});

	changeInvincibility(0.5f);
}

std::shared_ptr<Building> Building::create(BuildingID id, sf::Vector2f pos, int _faction) {
    std::shared_ptr<Building> building = std::make_shared<Building>(id, pos, _faction);
    building->initializePtr(building);
    building->initializeHitbox();
    return building;
}

void Building::initializePtr(std::shared_ptr<Building> ptr) {
    selfPtr = ptr;
}

void Building::initializeHitbox() {
    entityHitbox = std::make_shared<Hitbox>(selfPtr, position, sprite.getGlobalBounds().size, faction);
    CollisionManager::getInstance().addEntityHitbox(entityHitbox);
}

void Building::onCollision(float damage, sf::Vector2f damageOrigin) {
	if (isDestructable && fromCollision > invincibility) {
		
        fromCollision = 0.f;
		if (health > damage) { 
			health -= damage; 
			AudioManager::getInstance().play("townHurt", false, 50.f);
			sprite.setColor(sf::Color(255,255,255,155));
		}
		else {
			health = 0;
			isAlive = false;
			AudioManager::getInstance().play("townDestroy", false, 50.f);
			sprite.setColor(sf::Color(255,255,255,255));
		}
	}
}

void Building::update(float dt) {
	if (fromCollision < 3.0f) fromCollision += dt;
	if (fromCollision > 0.1f) {
		sprite.setColor(sf::Color(255,255,255,255));
	}
}

void Building::update(float dt, EnemyManager& enemyManager) {
	update(dt);
}

void Building::render(sf::RenderWindow& window) {
    window.draw(sprite);
	entityHitbox->render(window);

}

bool Building::isExpired() const {
	// not returning expired, because there is nothing between alive and expired yet
	return !isAlive;
}

const sf::Vector2f Building::getPosition() const {
	return position;
}