#include "Hitbox.hpp"
#include "Entity.hpp"
#include "CollisionManager.hpp"
#include <cmath>
#include <iostream>


// ------- Hitbox -------

Hitbox::Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, int _faction, bool _isVisible)
	:shapeType(0), id(0), lifetime(0.f), finished(false), isVisible(_isVisible), faction(_faction)
{
	rectHB.setSize(size);
	rectHB.setOrigin( size/2.f );
	rectHB.setPosition(position);
	rectHB.setFillColor(sf::Color(255, 255, 0, 150));

	circleHB.setPosition({ 0.f ,0.f });
	circleHB.setRadius(0.f);


	hitboxOwner = owner;
	faction = owner.lock()->getFaction();
}


Hitbox::Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, int _faction, bool _isVisible)
	:shapeType(1), id(0), lifetime(0.f), finished(false), isVisible(_isVisible), faction(_faction)
{
	circleHB.setRadius(r);
	circleHB.setOrigin({ r, r });
	circleHB.setPosition(position);
	circleHB.setFillColor(sf::Color(255, 255, 0, 150));

	rectHB.setPosition({ 0.f, 0.f });
	rectHB.setSize({ 0.f, 0.f });

	hitboxOwner = owner.lock();
}

void Hitbox::setID(int _id) {
	id = _id;
}

int Hitbox::getID() {
	return id;
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

// Updated the hitbox position and returns the modified movement vector
// Maybe add arguments for damage to trigger onCollision() here?
sf::Vector2f Hitbox::updateHitbox(sf::Vector2f movementVec, bool doCollision, float damage) {
	sf::CircleShape movedCircX = circleHB;
    sf::CircleShape movedCircY = circleHB;
	sf::RectangleShape movedRectX = rectHB;
	sf::RectangleShape movedRectY = rectHB;
	sf::Vector2f modifiedMovementVec = movementVec;

	const std::vector<std::weak_ptr<Hitbox>>& entityHitboxes = CollisionManager::getInstance().getEntityHitboxList();
    for (const auto& hitbox : entityHitboxes) {
		bool collided = false;

        // skip own or expired hitbox
        if (hitbox.expired()) continue;
        auto hb = hitbox.lock();
        if (id == hb->getID()) continue;

		if (shapeType == 0) {
			movedRectX.move({ modifiedMovementVec.x, 0.f });
			movedRectY.move({ 0.f, modifiedMovementVec.y });

			// if hitbox type is rectangle
			if (hb->getType() == 0) {
				const auto& rect = hb->getRectHitbox().getGlobalBounds();
				if ( movedRectX.getGlobalBounds().findIntersection(rect) ) {
					// if collision on x axis, make x movement 0
					modifiedMovementVec.x = 0.f;
					collided = true;
				}
				if ( movedRectY.getGlobalBounds().findIntersection(rect) ) {
					// if collision on y axis, make y movement 0
					modifiedMovementVec.y = 0.f;
					collided = true;
				}
			}
			// if hitbox type is circle 
			else if (hb->getType() == 1) {
				const sf::CircleShape& circle = hb->getCircleHitbox();

				if (CollisionManager::checkCircleRectCollision(circle, movedRectX)) {
					// if collision on x axis, make x movement 0
					modifiedMovementVec.x = 0.f;
					collided = true;
				}
				if (CollisionManager::checkCircleRectCollision(circle, movedRectX)) {
					// if collision on y axis, make y movement 0
					modifiedMovementVec.y = 0.f;
					collided = true;
				}
			}
		}
		else if (shapeType == 1) {
			movedCircX.move({ modifiedMovementVec.x, 0.f });
			movedCircY.move({ 0.f, modifiedMovementVec.y });

			// if hitbox type is rectangle
			if (hb->getType() == 0) {
				const sf::RectangleShape& rect = hb->getRectHitbox();
				if (CollisionManager::checkCircleRectCollision(movedCircX, rect)) {
					// if collision on x axis, make x movement 0
					modifiedMovementVec.x = 0.f;
					collided = true;
				}
				if (CollisionManager::checkCircleRectCollision(movedCircY, rect)) {
					// if collision on y axis, make y movement 0
					modifiedMovementVec.y = 0.f;
					collided = true;
				}
			}
			// if hitbox type is circle 
			else if (hb->getType() == 1) {
				const sf::CircleShape& circle = hb->getCircleHitbox();
				
				if (CollisionManager::checkCircleCollision(movedCircX, circle) || CollisionManager::checkCircleCollision(movedCircY, circle)) {
					modifiedMovementVec = CollisionManager::slideAgainstCircle(movedCircX, circle, modifiedMovementVec);
					collided = true;
				}
			}
		}

		if (collided && doCollision && (faction != hb->getFaction()) ) {
			hb->onCollision(damage, getPosition());
			hitboxOwner.lock()->onAttack();
        }

	}

	// move the hitbox using the modified movement vector
	if (shapeType == 0) rectHB.move(modifiedMovementVec);
	else if (shapeType == 1) circleHB.move(modifiedMovementVec);

	return modifiedMovementVec;
}

void Hitbox::changeVisibility(bool vis) {
	isVisible = vis;
}

void Hitbox::onCollision(float damage, sf::Vector2f damageSource) {
	if (auto entity = hitboxOwner.lock()) {
		// auto owner = hitboxOwner.lock();
		// std::cout << owner->getPosition().x << ", " << owner->getPosition().y << std::endl;
		entity->onCollision(damage, damageSource);
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
				if ((isColliding = CollisionManager::checkCircleRectCollision(entityHB->getCircleHitbox(), rectHB))) {
					//std::cout << "circ rect" << std::endl;
				}
			}
			//std::cout << isColliding << std::endl;
			if (isColliding) entityHB->onCollision(damage, thisOwner->getPosition());
			++entityIteration;
		}
	}
	CollisionManager::getInstance().removeExpiredHitboxes();
}


