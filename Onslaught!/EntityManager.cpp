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
	: nextEntityID(0), playerPositionHolder({0.f,0.f})
{
	itemManager = ItemManager();
	buildManager = BuildingManager();
	playerInventory = InventoryManager(15);
	enemyManager = EnemyManager();
	objectManager = ObjectManager();
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
	enemyManager.setCamera(cam);
	objectManager.setCamera(cam);
}

void EntityManager::reset() {
	allEntities.clear();
	CollisionManager::getInstance().clear();
	enemyManager.reset();
	itemManager.reset();
	buildManager.reset();
	objectManager.reset();
	playerInventory.reset();
	nextEntityID = 0;
	score = 0;
	gameTime = 0.f;
	playerAlive = false;
	townAlive = false;

	// start with items in inventory for testing/debugging
	playerInventory.addItem(ItemID::Wood, 200);
	playerInventory.addItem(ItemID::Stone, 200);
	playerInventory.printItems();
}

void EntityManager::clear() {
	allEntities.clear();
	CollisionManager::getInstance().clear();
	itemManager.reset();
	buildManager.reset();
	objectManager.reset();
	playerInventory.reset();
	window = nullptr;
	camera = nullptr;
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

	//std::cout << "Running" << std::endl;
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
	itemManager.update(dt, playerPositionHolder, playerInventory, player->getStatusManager());
	buildManager.update(dt, playerPositionHolder, playerInventory, enemyManager, _window);
	enemyManager.update(dt, buildManager);
	objectManager.update(dt);

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
			it = allEntities.erase(it);   // deletes shared_ptr
		}
		else {
			++it;
		}
	}
	gameTime += dt;
}

// must be called after updates
void EntityManager::renderAlive(sf::RenderWindow& _window) {
	for (auto& ent : allEntities) {
		ent->render(_window);
	}
	enemyManager.render(_window);
	itemManager.render(_window);
	buildManager.render(_window);
	objectManager.render(_window);
}

sf::Vector2f EntityManager::getPlayerPos() const {
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


int EntityManager::getScore() const {
	return score;
}

void EntityManager::incrementScore() {
	score++;
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

	std::weak_ptr<Town> EntityManager::getTownPtr() const {
	return townPtr;
}

void EntityManager::spawnItems(ItemID id, const sf::Vector2f& pos, int qty) {
	itemManager.spawnItem(id, pos, qty);
}

void EntityManager::spawnExp(const sf::Vector2f& pos, int qty) {
	itemManager.spawnExp(pos, qty);
}

int EntityManager::getNextEntityID() {
	int id = nextEntityID;
	nextEntityID++;
	return id;
}

const InventoryManager& EntityManager::getPlayerInventory() const {
	return playerInventory;
}

void EntityManager::updateInventoryTextures(float screenW, float screenH) {
	if(!inventoryUpdated) playerInventory.updateSlotTextures(screenW, screenH);
	inventoryUpdated = true;
}

void EntityManager::setInvUpdate(bool updated) {
	inventoryUpdated = updated;
}