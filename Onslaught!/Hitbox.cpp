#include "Hitbox.hpp"
#include "Entity.hpp"
#include "CollisionManager.hpp"
#include <cmath>
#include <iostream>


// ------- Hitbox -------

Hitbox::Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, int _faction, bool _isVisible)
	:shapeType(0), lifetime(0.f), finished(false), isVisible(_isVisible), faction(_faction)
{
	rectHB.setSize(size);
	rectHB.setOrigin( size/2.f );
	rectHB.setPosition(position);
	rectHB.setFillColor(sf::Color::Yellow);

	circleHB.setPosition({ 0.f ,0.f });
	circleHB.setRadius(0.f);

	hitboxOwner = owner;
	faction = owner.lock()->getFaction();
}


Hitbox::Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, int _faction, bool _isVisible)
	:shapeType(1), lifetime(0.f), finished(false), isVisible(_isVisible), faction(_faction)
{
	circleHB.setRadius(r);
	circleHB.setOrigin({ r, r });
	circleHB.setPosition(position);
	circleHB.setFillColor(sf::Color::Yellow);

	rectHB.setPosition({ 0.f, 0.f });
	rectHB.setSize({ 0.f, 0.f });

	hitboxOwner = owner.lock();
}


void Hitbox::update(float dt) { lifetime += dt; }

void Hitbox::render(sf::RenderWindow& window) const {
	if (isVisible) {
		if (!finished && shapeType == 0) window.draw(rectHB);
		if (!finished && shapeType == 1) window.draw(circleHB);
	}
}
bool Hitbox::isExpired() const { return finished; }

int Hitbox::getType() const {
	if (shapeType > 1 || shapeType < 0) {
		std::cout << "Invalid shape type!" << std::endl;
		return 0;
	}
	return shapeType; 
}

sf::Vector2f Hitbox::getPosition() {
	if (shapeType == 0) return rectHB.getPosition();
	else if (shapeType == 1) return circleHB.getPosition();
	std::cout << "Shape type invalid!" << std::endl;

	return { 0.f, 0.f };
}

// might not need virtual for these
sf::Vector2f Hitbox::getSize() {
	if (shapeType == 0) return rectHB.getSize();
	else std::cout << "Hitbox not a rectangle" << std::endl;

	return { 0.f, 0.f };
}

float Hitbox::getRadius() {
	if (shapeType == 1) return circleHB.getRadius();
	else std::cout << "Hitbox not a circle" << std::endl;

	return 0.f;
}

const sf::RectangleShape& Hitbox::getRectHitbox() {
	if (shapeType == 0) return rectHB;
	else {
		std::cout << "Hitbox not a rectangle" << std::endl;
		return rectHB;	//empty rect
	}
}

const sf::CircleShape& Hitbox::getCircleHitbox() {
	if (shapeType == 1) return circleHB;
	else {
		std::cout << "Hitbox not a circle" << std::endl;
		return circleHB;	//empty circle
	}
}

// that or use a movement vector pass
void Hitbox::updateHitbox(sf::Vector2f newPosition) {
	if (shapeType == 0) rectHB.setPosition(newPosition);
	else if (shapeType == 1) circleHB.setPosition(newPosition);
}

void Hitbox::changeVisibility(bool vis) {
	isVisible = vis;
}

void Hitbox::onCollision(float damage) {
	if (auto entity = hitboxOwner.lock()) {
		entity->onCollision(damage);
	}
}

void Hitbox::setFaction(int _faction) {
	if (_faction < 0 || 2 < _faction) {
		faction = 0;
	}
	else {
		faction = _faction;
	}
}

int Hitbox::getFaction() {
	return faction;
}

std::weak_ptr<Entity> Hitbox::getOwner() {
	return hitboxOwner;
}

// ------- Trigger hitbox -------

// Constructor for rectangle trigger hitbox
TriggerHitbox::TriggerHitbox(std::weak_ptr<Entity> _owner, sf::Vector2f position, sf::Vector2f size, float _damage, float _speed, float _range, int _faction)
	:Hitbox(_owner, position, size, _faction), damage(_damage), speed(_speed), attackRange(_range) {
}
// Constructor for circle trigger hitbox
TriggerHitbox::TriggerHitbox(std::weak_ptr<Entity> _owner, sf::Vector2f position, float radius, float _damage, float _speed, float _range, int _faction)
	:Hitbox(_owner, position, radius, _faction), damage(_damage), speed(_speed), attackRange(_range) {
}
void TriggerHitbox::updateTriggerHitbox(float deltaTime, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection) {
	lifetime += deltaTime;
	return;
}


// ------- Swing Attack Hitbox (Rect Hitbox Only) -------

// speed -> swing speed (degrees), range (swing range)
SwingHitbox::SwingHitbox(std::weak_ptr<Entity> _owner, sf::Vector2f position, sf::Vector2f size, float damage, float speed, float range, int _faction)
	:TriggerHitbox(_owner, position, size, damage, speed, range, _faction) {
	auto owner = _owner.lock();
	offsetRadius = (size.x / 2.f) + owner->getSize();
	initialAngle = owner->getDirection() - range / 2.f;
	currentAngle = 0.f;
}

void SwingHitbox::updateTriggerHitbox(float dt, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection) {
	if (finished) return;

	currentAngle += speed * dt;

	//currentAngle >= initialAngle+attackRange || currentAngle > maxSwing
	if (currentAngle >= attackRange) {
		finished = true;
		return;
	}
	// calculate the rad from current angle and start angle
	float totalAngle = initialAngle + currentAngle;
	float rad = totalAngle * 3.14159f / 180.f;
	// rotate around the player center
	sf::Vector2f offset(std::cos(rad) * offsetRadius, std::sin(rad) * offsetRadius);
	// update player position as well so the hitbox moves along with player
	if (shapeType == 0) {
		rectHB.setPosition(currentPlayerPos + offset);
		rectHB.setRotation(sf::degrees(totalAngle));
	}

	// check for collision on each hitbox, not in collision manager (struggling with owners of hitboxes)
	std::vector<std::weak_ptr<Hitbox>>& entityHitboxes = CollisionManager::getInstance().getEntityHitboxList();
	
	auto thisOwner = hitboxOwner.lock();
	for (auto entityIteration = entityHitboxes.begin(); entityIteration != entityHitboxes.end();) {
		if (entityIteration->expired()) {
			++entityIteration;
			continue;
		}
		auto entityHB = entityIteration->lock();
		auto entOwner = entityHB->hitboxOwner.lock();

		if (thisOwner && entOwner && (thisOwner == entOwner)) {
			//std::cout << "Skip" << std::endl;
			++entityIteration;
			continue;
		}
		else if (faction == entOwner->getFaction()) {
			++entityIteration;
			continue;
		}
		else
		{
			bool isColliding = false;
			if (entityHB->getType() == 0) {
				if (rectHB.getGlobalBounds().findIntersection(entityHB->getRectHitbox().getGlobalBounds())) {
					isColliding = true;
					//std::cout << "rect rect" << std::endl;
				}
			}
			else if (entityHB->getType() == 1) {
				if (isColliding = CollisionManager::checkCircleRectCollision(entityHB->getCircleHitbox(), rectHB)) {
					//std::cout << "circ rect" << std::endl;
				}
			}
			//std::cout << isColliding << std::endl;
			if (isColliding) entityHB->onCollision(damage);
			++entityIteration;
		}
	}
	CollisionManager::getInstance().removeExpiredHitboxes();
}


