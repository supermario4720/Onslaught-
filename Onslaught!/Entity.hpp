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

public:
	// Entity constructer without specifying current health
	Entity(float _maxHealth, bool destructable = true, bool alive = true)
		: maxHealth(_maxHealth), health(_maxHealth), isAlive(alive), isDestructable(destructable), isDying(false),
		entityID(0), typeID(0), faction(0),
		fromCollision(0.f), invincibility(0.2f) 
	{}
	// constructer with health specification
	Entity(float _maxHealth, float _health, bool destructable = true, bool alive = true)
		: maxHealth(_maxHealth), health(_health <= 0 ? 0.f : _health), isAlive(alive), isDestructable(destructable), isDying(false),
		entityID(0), typeID(0), faction(0),
		fromCollision(0.f), invincibility(0.2f)
	{}

	virtual void initializeHitbox() {}

	virtual void onCollision(float damage) {
		std::cout << "base entity collision" << std::endl;
	}

	virtual void update(float dt) {
		std::cout << "calling base entity update" << std::endl;
	}

	virtual float getSize() { return 30.f; }

	virtual float getDirection() { return 0.f; }

	virtual void render(sf::RenderWindow& window) {}

	virtual bool checkAlive() {
		return isAlive;
	}

	virtual void onDeath() {};

	virtual void changeDestructability(bool dest) {
		isDestructable = dest;
	}

	virtual void changeInvincibility(float invincTime) {
		invincibility = invincTime;
	}

	// get ID of entity instance
	int getEntityID() const {
		return entityID;
	}
	// set entity instance ID
	void setEntityID(int id) {
		entityID = id;
	}
	// get entity type ID
	int getTypeID() const {
		return typeID;
	}
	// set entity type ID
	void setTypeID(int id) {
		typeID = id;
	}
	// get faction ID
	int getFaction() const {
		return faction;
	}
	// set faction ID
	void setFaction(int _faction) {
		faction = _faction;
	}

	virtual ~Entity() = default;

};