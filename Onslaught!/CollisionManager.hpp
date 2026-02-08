#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include "Hitbox.hpp"


class CollisionManager {
private:
	// contains hitboxes that can't overlap (blocking hitboxes)
	std::vector<std::weak_ptr<Hitbox>> entityHitboxes;
	int nextID = 0;

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

	// check if shape collides with any entity
	bool checkAllCollision(sf::CircleShape& self);
	// check if shape collides with any entity
	bool checkAllCollision(sf::RectangleShape& self);

	//static void handleCollision(Player& player, std::vector<sf::RectangleShape> objects);

	void clear();

};




