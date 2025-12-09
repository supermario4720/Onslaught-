#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include "Hitbox.hpp"

// static variables in classes are owned by the class itself
//	-> it doesnt need an instance to exist, so it can be called directly
//		(e.g. Class::varName; )

class CollisionManager {
private:
	// list of hitboxes that CANNOT overlap (for moving entities to check if destination is free or not)
	std::vector<std::weak_ptr<Hitbox>> entityHitboxes;
	// vector list of trigger hitboxes
	// std::vector<std::weak_ptr<TriggerHitbox>>triggerHitboxes;

public:
	static CollisionManager& getInstance();

	~CollisionManager();

	//void addTriggerHitbox(std::shared_ptr<TriggerHitbox> hb);
	void addEntityHitbox(std::shared_ptr<Hitbox> hb);
	//void removeTriggerHitbox(std::weak_ptr<TriggerHitbox> hb);
	void removeEntityHitbox(std::weak_ptr<Hitbox> hb);

	void removeExpiredHitboxes();

	std::vector<std::weak_ptr<Hitbox>>& getEntityHitboxList(){
		return entityHitboxes;
	}


	/*
	const std::vector<std::weak_ptr<TriggerHitbox>>& getHitboxList() const {
		return triggerHitboxes;
	}
	*/

	void update(float dt);

	// collision checking functions

	static bool checkCircleCollision(const sf::CircleShape& self, const sf::CircleShape& target);

	static bool checkCircleRectCollision(const sf::CircleShape& circle, const sf::RectangleShape& rect);

	static sf::Vector2f slideAgainstCircle(sf::CircleShape& self, const sf::CircleShape& target, sf::Vector2f movementVector);

	//static void handleCollision(Player& player, std::vector<sf::RectangleShape> objects);

	void clear();

};




