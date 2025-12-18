#pragma once
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include "ItemManager.hpp"
#include "InventoryManager.hpp"
#include "BuildingManager.hpp"
#include "BuildingID.hpp"
#include <vector>
#include <memory>
#include <string>

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

	std::vector<std::shared_ptr<Entity>> allEntities;
	std::vector<std::shared_ptr<Building>> buildings;
	std::shared_ptr<Player> player;
	std::weak_ptr<Town> townPtr;
	Camera* camera = nullptr;

	sf::Vector2f playerPositionHolder;

	int nextEntityID = 0;

	int enemyCount = 0;
	int maxEnemies = 10;
	float spawnTimer = 0.f;
	float nextInterval = 1.f;
	float minSpawnInterval = 2.f;
	float maxSpawnInterval = 10.f;

	int score = 0;
	float gameTime = 0.f;
	bool playerAlive = false;
	bool townAlive = false;

public:
	static EntityManager& getInstance();

	void setWindow(sf::RenderWindow* _window);

	void setCamera(Camera* cam);

	void reset();

	void clear();

	void start(Camera* cam);

	void update(float dt, sf::RenderWindow& _window);

	void spawnEnemies(float dt, std::shared_ptr<Town>& town);

	void renderAlive(sf::RenderWindow& _window);

	sf::Vector2f getPlayerPos();

	float randf(float min, float max);

	sf::Vector2f randomSpawnPosition(const sf::View& camera, std::shared_ptr<Town>& town);

	bool isTownOnSpawnPos(sf::Vector2f spawnPos, std::shared_ptr<Town>& town);

	void spawnItems(ItemID id, const sf::Vector2f& pos, int qty);

	void createBuilding(BuildingID id, sf::Vector2f& pos, int _faction);

	int getScore() const;
	float getTime() const;
	bool isPlayerAlive() const;
	bool isTownAlive() const;

	// return 2 floats (vector2f) of current/max hp
	sf::Vector2f getPlayerHealth();
	// return 2 floats (vector2f) of current/max stamina
	sf::Vector2f getPlayerStamina();

};