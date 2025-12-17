#include <SFML/Graphics.hpp>
#include "EntityManager.hpp"
#include "Camera.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Object.hpp"
#include "Town.hpp"

#include "Building.hpp"

#include "CollisionManager.hpp"
#include "GameStateManager.hpp"


EntityManager::EntityManager()
	: nextEntityID(0), enemyCount(0), maxEnemies(10), 
	spawnTimer(0.f), nextInterval(1.f), 
	minSpawnInterval(1.f), maxSpawnInterval(5.f),
	playerPositionHolder({0.f,0.f})
{
	itemManager = ItemManager();
	buildManager = BuildingManager();
	playerInventory = InventoryManager();
}

EntityManager& EntityManager::getInstance() {
	static EntityManager instance;
	return instance;
}

void EntityManager::setWindow(sf::RenderWindow* _window) {
	window = _window;
}

void EntityManager::setCamera(Camera* cam) {
	camera = cam;
}

void EntityManager::reset() {
	allEntities.clear();
	CollisionManager::getInstance().clear();
	itemManager.reset();
	buildManager.reset();
	playerInventory.reset();
	nextEntityID = 0;
	enemyCount = 0;
	score = 0;
	gameTime = 0.f;
	playerAlive = false;
	townAlive = false;
}

void EntityManager::start(Camera* cam) {
	reset();
	std::shared_ptr<Town> town = Town::createTown();
	town->setEntityID(0);
	townPtr = town;

	player = Player::create();
	player->setEntityID(1);
	nextEntityID = 2;

	allEntities.push_back(town);
	allEntities.push_back(player);
	playerAlive = true;
	townAlive = true;
	setCamera(cam);
}

void EntityManager::update(float dt, sf::RenderWindow& _window) {
	auto town = townPtr.lock();

	// if player or town doesn't exist or is not alive, return
	if (!player || !player->checkAlive() ) {
		playerAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	if (!town || !town->checkAlive()) {
		townAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	playerPositionHolder = player->getPosition();
	itemManager.update(dt, playerPositionHolder, playerInventory);
	buildManager.update(dt, playerPositionHolder, playerInventory, _window);

	// spawn Enemies
	spawnEnemies(dt, town);
	// update all entites
	player->updatePlayer(dt, buildManager);
	for (auto& ent : allEntities) {
		ent->update(dt);
	}
	// return if player or town died after update
	if (!player->checkAlive()) {
		playerAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}
	if (!town->checkAlive()) {
		townAlive = false;
		GameStateManager::getInstance().setState(GameStateManager::State::GameOver);
		return;
	}

	// check if player near town
	town->checkPlayerNear(player->getPosition());
	if (town->isPlayerNear()) {
		// player->interactingWithTown();
		// std::cout << "Player is near Town" << std::endl;
		town->setColor(sf::Color(105, 105, 105));
	}
	else { town->setColor(sf::Color(155, 155, 155)); }

	for (auto it = allEntities.begin(); it != allEntities.end(); ) {
		if (!(*it)->checkAlive()) {
			if( (*it)->getFaction() == 1) {
				enemyCount--;
				score++;
			}
			it = allEntities.erase(it);   // deletes shared_ptr
		}
		else {
			++it;
		}
	}
	gameTime += dt;
}

void EntityManager::spawnEnemies(float dt, std::shared_ptr<Town>& town) {
	// spawn enemy at random interval if below 10
	if (enemyCount < maxEnemies) {
		if (spawnTimer < nextInterval) {
			spawnTimer += dt;
		}
		else {
			sf::Vector2f pos = randomSpawnPosition((*camera).getView(), town);
			std::shared_ptr<Enemy> newEnemy = Enemy::create(pos);
			newEnemy->setEntityID(nextEntityID);
			nextEntityID++;
			allEntities.push_back(newEnemy);

			nextInterval = randf(minSpawnInterval, maxSpawnInterval);
			spawnTimer = 0.f;
			enemyCount++;
		}
	}
}

// must be called after updates
void EntityManager::renderAlive(sf::RenderWindow& _window) {
	for (auto& ent : allEntities) {
		ent->render(_window);
	}
	for (auto& building : buildings) {
		building->render(_window);
	}

	itemManager.render(_window);
	buildManager.render(_window);
}

sf::Vector2f EntityManager::getPlayerPos() {
	// if player ptr is null/empty
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 0.f };
	}
	else
	{
		return player->getPosition();
	}
}

float EntityManager::randf(float min, float max) {
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

sf::Vector2f EntityManager::randomSpawnPosition(const sf::View& camera, std::shared_ptr<Town>& town) {
	sf::Vector2f viewPos = camera.getCenter();
	sf::Vector2f viewSize = camera.getSize();
	float marginSize = 50.f;

	float left = viewPos.x - viewSize.x * 0.5f - marginSize;
	float right = viewPos.x + viewSize.x * 0.5f + marginSize;
	float top = viewPos.y - viewSize.y * 0.5f - marginSize;
	float bottom = viewPos.y + viewSize.y * 0.5f + marginSize;

	int attempts = 0;
	while (attempts < 15) {
		int side = rand() % 4;
		sf::Vector2f spawnPosition;

		switch (side)
		{
			case 0: // left side
				spawnPosition = { left, randf(top, bottom) };
				break;

			case 1: // right side
				spawnPosition = { right, randf(top, bottom) };
				break;

			case 2: // top
				spawnPosition = { randf(left, right), top };
				break;
			case 3: // bottom
			default:
				spawnPosition = { randf(left, right), bottom };
				break;
		}

		if (!isTownOnSpawnPos(spawnPosition, town)) {
			return spawnPosition;
		}
		attempts++;
	}

	// If good position not found within attempts, return position farthest from town
	sf::Vector2f corners[4] = {
	{left, top},
	{right, top},
	{left, bottom},
	{right, bottom}
	};
	sf::Vector2f townPos = town->getTownPosition();

	float maxDist = 0.f;
	sf::Vector2f bestCorner = corners[0];
	for (const auto& corner : corners) {
		float dx = corner.x - townPos.x;
		float dy = corner.y - townPos.y;
		float dist = std::sqrt(dx * dx + dy * dy);
		if (dist > maxDist) {
			maxDist = dist;
			bestCorner = corner;
		}
	}

	return bestCorner;
}

bool EntityManager::isTownOnSpawnPos(sf::Vector2f spawnPos, std::shared_ptr<Town>& town) {
	sf::Vector2f townPos = town->getTownPosition();
	sf::Vector2f townSize = town->getTownSize();  // Returns {width, height}
	float buffer = 100.f;

	// Rectangular bounds check (no sqrt needed!)
	float halfWidth = townSize.x / 2.f + buffer;
	float halfHeight = townSize.y / 2.f + buffer;

	float dx = std::abs(spawnPos.x - townPos.x);
	float dy = std::abs(spawnPos.y - townPos.y);

	return (dx < halfWidth && dy < halfHeight);
}

int EntityManager::getScore() const {
	return score;
}

float EntityManager::getTime() const {
	return gameTime;
}

bool EntityManager::isPlayerAlive() const {
	return playerAlive;
}

bool EntityManager::isTownAlive() const {
	return townAlive;
}

sf::Vector2f EntityManager::getPlayerHealth() {
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 1.f };
	}
	else {
		return player->getHealth();
	}
}


sf::Vector2f EntityManager::getPlayerStamina() {
	if (!player) {
		std::cout << "Player doesn't exist!" << std::endl;
		return { 0.f, 1.f };
	}
	else {
		return player->getStamina();
	}
}

void EntityManager::spawnItems(ItemID id, const sf::Vector2f& pos, int qty) {
	itemManager.spawn(id, pos, qty);
}

void EntityManager::createBuilding(BuildingID id, sf::Vector2f& pos, int _faction) {
	buildings.push_back( Building::create(id, pos, _faction) );
}