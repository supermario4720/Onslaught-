#pragma once
#include "Enemy.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Camera;
class BuildManager;

class EnemyManager {
public:
    EnemyManager();

    void setCamera(Camera* ptr);

    // make it so that the function only needs to be called, and no params needed
        // building ID should be stored in the manager, with a select (update function updates the selected building on input)
    void spawnEnemy(float dt);

    float randf(float min, float max);

	sf::Vector2f randomSpawnPosition();

    void update(float dt, const BuildingManager& buildManager);

    void render(sf::RenderWindow& window);

    void reset();

    const std::vector<std::shared_ptr<Enemy>>& getEnemyList() const;

private:
    std::vector<std::shared_ptr<Enemy>> enemies;

    Camera* cameraPtr = nullptr;

    // replace with radius from enemy database if made later
    float enemySize = 30.f;

    int enemyCount = 0;
	int maxEnemies = 10;
	float spawnTimer = 0.f;
	float nextInterval = 1.f;
	float minSpawnInterval = 2.f;
	float maxSpawnInterval = 10.f;

};
