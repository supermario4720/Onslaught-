#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

// base class for all entities
class Entity : public std::enable_shared_from_this<Entity> {
protected:
	float maxHealth;
	float health;
	bool isAlive;
	bool isDying;
	bool isDestructable;

	float fromCollision;
	float invincibility;

	// Each instance of entity should be assigned a unique identifier
	int entityID;
	// Each type of entity (e.g. player, enemy) should have type identifier
	int typeID;
	// Each entity should belong to a faction (ally, enemy, neutral, etc), preventing friendly fire
	// 0=player, 1=enemy, 2=object
	int faction;

	float spriteOffset = -15.f;

public:
	// Entity constructer without specifying current health
	Entity(float _maxHealth, bool destructable = true, bool alive = true);
	// constructer with health specification
	Entity(float _maxHealth, float _health, bool destructable = true, bool alive = true);

	virtual void initializeHitbox();

	virtual void onCollision(float damage);

	virtual void update(float dt);

	virtual float getSize();

	virtual float getDirection();

	virtual void render(sf::RenderWindow& window);

	virtual bool checkAlive();

	virtual void onDeath();

	virtual void changeDestructability(bool dest);

	virtual void changeInvincibility(float invincTime);

	void setMaxHealth(float maxhp);

	// get ID of entity instance
	int getEntityID() const;

	// set entity instance ID
	void setEntityID(int id);

	// get entity type ID
	int getTypeID() const;

	// set entity type ID
	void setTypeID(int id);
	// get faction ID
	int getFaction() const;
	// set faction ID
	void setFaction(int _faction);

	virtual ~Entity() = default;

};