#include <SFML/Graphics.hpp>
#include "Town.hpp"
#include "Hitbox.hpp"
#include "CollisionManager.hpp"
#include "AudioManager.hpp"
//#include <iostream>


Town::Town()
	:Entity(200.f), playerNear(false), interactionRadius(50.f),
	normalTexture("resources/Castle_Blue.png"),damageTexture("resources/Castle_Red.png"), 
	destroyedTexture("resources/Castle_Destroyed.png"), sprite(normalTexture)
{
	// setting corresponding IDs
	setTypeID(0);
	setFaction(0);

	sf::Vector2f size({200.f, 125.f});
	rect.setSize(size);
	rect.setOrigin(size/2.f);
	rect.setPosition({ 0.f,0.f });
	rect.setFillColor(sf::Color(155, 155, 155));


	sprite.setTexture(normalTexture);
	sf::Vector2f spriteSize = sprite.getGlobalBounds().size;
    sprite.setOrigin({spriteSize.x / 2.f, spriteSize.y/2.f});

	float scale = (size.x*1.1f / spriteSize.x);
	sprite.setScale({scale, scale});

    sprite.setPosition({0.f, 0.f - spriteOffset});

	// interaction radius should be AT MINIMUM player rad + townSize/2
	interactionRadius = 100.f;

	changeInvincibility(0.5f);

	//changeDestructability(false);
}

Town::~Town() {}

	// fully construct and return shared ptr of Town instance
std::shared_ptr<Town> Town::createTown() {
	std::shared_ptr<Town> town = std::shared_ptr<Town>(new Town());
	town->initializePtr(town);
	town->initializeHitbox();
	return town;
}

void Town::initializePtr(std::shared_ptr<Town> town) {
	selfPtr = town;
}

void Town::initializeHitbox() {
	townHB = std::make_shared<Hitbox>(selfPtr, rect.getPosition(), rect.getSize(), 0);
	townHB->changeVisibility(false);
	CollisionManager::getInstance().addEntityHitbox(townHB);
}


void Town::onCollision(float damage) {
	if (isDestructable && fromCollision > invincibility) {

		rect.setFillColor(sf::Color::White);
		fromCollision = 0.f;

		if (health > damage) { 
			health -= damage; 
			AudioManager::getInstance().play("townHurt", false, 50.f);
			sprite.setTexture(damageTexture);
			sprite.setColor(sf::Color(255,255,255,155));
		}
		else {
			health = 0;
			isAlive = false;
			AudioManager::getInstance().play("townDestroy", false, 50.f);
			sprite.setTexture(destroyedTexture);
			sprite.setColor(sf::Color(255,255,255,255));
		}
	}
}

void Town::update(float dt) {

	if (fromCollision < 3.0f) fromCollision += dt;
	if (fromCollision > 0.1f) {
		rect.setFillColor(sf::Color(155,155,155));

		sprite.setTexture(normalTexture);
		sprite.setColor(sf::Color(255,255,255,255));
	}
}

void Town::render(sf::RenderWindow& window) {
	//window.draw(rect);
	window.draw(sprite);
}

void Town::checkPlayerNear(sf::Vector2f playerPos) {
	sf::Vector2f townPos = rect.getPosition();

	float dx = townPos.x - playerPos.x;
	float dy = townPos.y - playerPos.y;
	float playerDist = std::sqrt(dx * dx + dy * dy);

	playerNear = (playerDist <= interactionRadius);
}

sf::Vector2f Town::getTownPosition() const {
	return rect.getPosition();
}

sf::Vector2f Town::getTownSize() const {
	return rect.getSize();
}

sf::FloatRect Town::getBounds() const {
	return sprite.getGlobalBounds();
}

bool Town::isPlayerNear() {
	return playerNear;
}

void Town::setColor(sf::Color color) {
	if (fromCollision > invincibility) {
		rect.setFillColor(color);
	}
}