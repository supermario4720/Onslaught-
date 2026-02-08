#include "Hitbox.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Object.hpp"
#include "CollisionManager.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>


CollisionManager& CollisionManager::getInstance() {
	static CollisionManager instance;
	return instance;
}

CollisionManager::~CollisionManager() {
}

void CollisionManager::addEntityHitbox(std::shared_ptr<Hitbox> hb) {
	hb->setID(nextID);
	nextID += 1;
	entityHitboxes.push_back(hb);
}


void CollisionManager::removeEntityHitbox(std::weak_ptr<Hitbox> hb) {
	auto target = hb.lock().get();

	entityHitboxes.erase(
		std::remove_if(entityHitboxes.begin(), entityHitboxes.end(),
			[&](const std::weak_ptr<Hitbox>& w)
			{
				return w.lock().get() == target;
			}),
		entityHitboxes.end()
	);
}


void CollisionManager::removeExpiredHitboxes() {
	entityHitboxes.erase(
		std::remove_if(entityHitboxes.begin(), entityHitboxes.end(),
			[](const std::weak_ptr<Hitbox>& w) { return w.expired(); }),
		entityHitboxes.end()
	);
}


/*
void CollisionManager::addTriggerHitbox(std::shared_ptr<TriggerHitbox> hb) {
	triggerHitboxes.push_back(hb);
}


void CollisionManager::removeTriggerHitbox(std::weak_ptr<TriggerHitbox> hb) {
	// vector(list).erase removes all values from the specified range
		// in this case, remove values from the new end point (where unused values gathered) to the true end or array
	triggerHitboxes.erase( std::remove(triggerHitboxes.begin(), triggerHitboxes.end(), hb ) );
}
*/

void CollisionManager::update(float dt) {

}

bool CollisionManager::checkCircleCollision(const sf::CircleShape& self, const sf::CircleShape& target) {
	sf::Vector2f selfPos = self.getPosition();
	sf::Vector2f targetPos = target.getPosition();
	float selfRadius = self.getRadius();
	float targetRadius = target.getRadius();

	float dx = selfPos.x - targetPos.x;
	float dy = selfPos.y - targetPos.y;

	float distanceSqr = dx * dx + dy * dy;
	float combinedRad = selfRadius + targetRadius;

	// boolean -> true if combinedRad^2 within or equal to distSqr
	return distanceSqr <= combinedRad * combinedRad;
}

// must have origin of circle at center
bool CollisionManager::checkCircleRectCollision(const sf::CircleShape& circle, const sf::RectangleShape& rect) {
	sf::Vector2f circleCenter = circle.getPosition();
	float radius = circle.getRadius();
	sf::FloatRect rectBounds = rect.getGlobalBounds();

	// find the closest point of the rectangle's edge to the circle (notations changed)
		//top -> .position.y	|| .left -> position.x	|| .width/height -> .size.x/y
	float closestX = std::max(rectBounds.position.x, std::min(circleCenter.x, rectBounds.position.x + rectBounds.size.x));
	float closestY = std::max(rectBounds.position.y, std::min(circleCenter.y, rectBounds.position.y + rectBounds.size.y));

	// find the difference of closest rect point to circle center
	float dx = circleCenter.x - closestX;
	float dy = circleCenter.y - closestY;
	
	// return boolean of it closest rect point is within the circle radius
	return (dx * dx + dy * dy) < (radius * radius);
}

// movementVector should already be multiplied by delta time
sf::Vector2f CollisionManager::slideAgainstCircle(sf::CircleShape& self, const sf::CircleShape& target, sf::Vector2f movementVector) {
	sf::Vector2f selfPosition = self.getPosition();
	sf::Vector2f targetPosition = target.getPosition();
	sf::Vector2f delta = selfPosition - targetPosition;

	float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	float minDist = self.getRadius() + target.getRadius();

	sf::Vector2f modifiedMoveVector = movementVector;

	// only run if objects are overlapping (closer then they should be)
	if (dist < minDist) {
		// Collision happened
		if (dist == 0) dist = 0.0001f;
		sf::Vector2f normal = delta / dist; // normalized

		// Remove movement along the normal
		float proj = modifiedMoveVector.x * normal.x + modifiedMoveVector.y * normal.y;
		if (proj < 0.f) // only if moving *into* the surface
			modifiedMoveVector -= proj * normal;
	}

	return modifiedMoveVector;
}

// Check if shape is colliding with any entity
bool CollisionManager::checkAllCollision(sf::CircleShape& self) {
	bool isColliding = false;

	for (auto entityIteration = entityHitboxes.begin(); entityIteration != entityHitboxes.end();) {
		// skip expired
		if (entityIteration->expired()) {
			++entityIteration;
			continue;
		}

		auto entityHB = entityIteration->lock();
		if (entityHB->getType() == 0) {
			isColliding = CollisionManager::checkCircleRectCollision(self, entityHB->getRectHitbox());
		}
		else if (entityHB->getType() == 1) {
			isColliding = CollisionManager::checkCircleCollision(self, entityHB->getCircleHitbox());
		}
		// if true, return and end function
		if(isColliding) return isColliding;
		++entityIteration;
	}
	return isColliding;
}

bool CollisionManager::checkAllCollision(sf::RectangleShape& self) {
	bool isColliding = false;

	for (auto entityIteration = entityHitboxes.begin(); entityIteration != entityHitboxes.end();) {
		// skip expired
		if (entityIteration->expired()) {
			++entityIteration;
			continue;
		}

		auto entityHB = entityIteration->lock();
		if (entityHB->getType() == 0) {
			if( self.getGlobalBounds().findIntersection(entityHB->getRectHitbox().getGlobalBounds()) ) {
				return true;
			}
		}
		else if (entityHB->getType() == 1) {
			isColliding = CollisionManager::checkCircleRectCollision( entityHB->getCircleHitbox(), self );
		}
		// if true, return and end function
		if(isColliding) return isColliding;
		++entityIteration;
	}
	return isColliding;
}


void CollisionManager::clear() {
	entityHitboxes.clear();
	nextID = 0;
}