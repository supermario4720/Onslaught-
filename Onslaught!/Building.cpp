#include "Building.hpp"
#include "CollisionManager.hpp"
#include "AudioManager.hpp"
#include <SFML/Graphics.hpp>


Building::Building(BuildingID id, sf::Vector2f pos, int _faction)
: Entity( BuildingDatabase::get(id).health ), buildingID(id), position(pos), faction(_faction),
sprite( BuildingTextures::getInstance().getTexture( BuildingDatabase::get(id).texture ) )
{}

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
    buildingHitbox = std::make_shared<Hitbox>(selfPtr, position, sprite.getLocalBounds().size, faction);
    buildingHitbox->changeVisibility(false);
    CollisionManager::getInstance().addEntityHitbox(buildingHitbox);
}

void Building::onCollision(float damage) {
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
