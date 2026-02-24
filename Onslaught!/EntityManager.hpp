#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include "ItemManager.hpp"
#include "InventoryManager.hpp"
#include "BuildingManager.hpp"
#include "EnemyManager.hpp"
#include "ObjectManager.hpp"
#include "BuildingID.hpp"
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>

class Entity;
class Camera;
class Player;
class Town;
class Building;

class EntityManager {
private:
	EntityManager();
	sf::RenderWindow* window = nullptr;

	ItemManager itemManager;
	BuildingManager buildManager;
	InventoryManager playerInventory;
	EnemyManager enemyManager;
	ObjectManager objectManager;

	std::vector<std::shared_ptr<Entity>> allEntities;
	std::shared_ptr<Player> player;
	std::weak_ptr<Town> townPtr;
	Camera* camera = nullptr;

	sf::Vector2f playerPositionHolder;

	int nextEntityID = 0;

	int score = 0;
	float gameTime = 0.f;
	bool playerAlive = false;
	bool townAlive = false;

	bool inventoryUpdated = false;

public:
	static EntityManager& getInstance();

	void setWindow(sf::RenderWindow* _window);

	void setCamera(Camera* cam);

	void reset();

	void clear();

	void start(Camera* cam);

	void update(float dt, sf::RenderWindow& _window);

	void renderAlive(sf::RenderWindow& _window);

	sf::Vector2f getPlayerPos() const;

	void spawnItems(ItemID id, const sf::Vector2f& pos, int qty);

	void spawnExp(const sf::Vector2f& pos, int qty);

	int getNextEntityID();

	int getScore() const;
	void incrementScore();
	float getTime() const;
	bool isPlayerAlive() const;
	bool isTownAlive() const;

	// return 2 floats (vector2f) of current/max hp
	sf::Vector2f getPlayerHealth();
	// return 2 floats (vector2f) of current/max stamina
	sf::Vector2f getPlayerStamina();

	std::weak_ptr<Town> getTownPtr() const;

	const InventoryManager& getPlayerInventory() const;

	void updateInventoryTextures(float screenW, float screenH);
	void setInvUpdate(bool updated);

};

inline int randInt(int min, int max) {
	int range = max - min;
	return min + ( std::rand() % range );
}

inline float randf(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}