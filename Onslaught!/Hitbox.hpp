#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>



class Entity;

class Hitbox {
protected:
	// 0 for rect, 1 for circle
	int shapeType;
	sf::RectangleShape rectHB;
	sf::CircleShape circleHB;
	float lifetime;
	bool finished;
	bool isVisible;
	// 0 = player, 1 = enemy, 2 = object
	int faction;

public:
	std::weak_ptr<Entity> hitboxOwner;
	// rectangle hitbox constructor
	Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, int faction = 0, bool _isVisible = false);
	// circle hitbox constructor
	Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, int faction = 0, bool _isVisible = false);

	virtual ~Hitbox() = default;

	virtual void update(float dt);

	virtual void render(sf::RenderWindow& window) const;

	virtual bool isExpired() const;

	virtual int getType() const;

	virtual sf::Vector2f getPosition();

	// might not need virtual for these
	virtual sf::Vector2f getSize();

	virtual float getRadius();

	virtual const sf::RectangleShape& getRectHitbox();
	
	virtual const sf::CircleShape& getCircleHitbox();

	// that or use a movement vector pass
	virtual void updateHitbox(sf::Vector2f newPosition);

	void changeVisibility(bool vis);
	// instead, get owner ptr and call collision from there
	virtual void onCollision(float damage);

	virtual void setFaction(int _faction);

	virtual int getFaction();

	virtual std::weak_ptr<Entity> getOwner();
};



// hitbox for attacks (trigger onCollision events)
class TriggerHitbox : public Hitbox {
protected:
	float damage;
	float speed;
	float attackRange;

public:
	// constructor -> position and size for hitbox size
	TriggerHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, float damage, float speed, float range, int _faction);
	TriggerHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, float damage, float speed, float range, int _faction);

	virtual void updateTriggerHitbox(float deltaTime, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection);	
};


class SwingHitbox : public TriggerHitbox {
private:
	// distance from player origin to hitbox origin
	float offsetRadius;
	float initialAngle;
	float currentAngle;

public:
	// constructor -> position and size for hitbox size
	SwingHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, float damage, float speed, float range, int _faction);

	void updateTriggerHitbox(float deltaTime, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection) override;

};