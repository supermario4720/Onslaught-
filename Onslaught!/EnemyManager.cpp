#include "EnemyManager.hpp"
#include "EntityManager.hpp"
#include "CollisionManager.hpp"
#include "Camera.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/View.hpp>


EnemyManager::EnemyManager()
:enemyCount(0), maxEnemies(10),
spawnTimer(0.f),
nextInterval(1.f),
minSpawnInterval(2.f),
maxSpawnInterval(10.f)
{}

void EnemyManager::setCamera(Camera* ptr) {
    cameraPtr = ptr;
}

void EnemyManager::spawnEnemy(float dt) {
    
	// spawn enemy at random interval if below 10
	if (enemyCount < maxEnemies) {
		if (spawnTimer < nextInterval) {
			spawnTimer += dt;
		}
		else {
			sf::Vector2f pos = randomSpawnPosition();
			std::shared_ptr<Enemy> newEnemy = Enemy::create(pos);

			newEnemy->setEntityID(EntityManager::getInstance().getNextEntityID());
			enemies.push_back(newEnemy);

			nextInterval = randf(minSpawnInterval, maxSpawnInterval);
			spawnTimer = 0.f;
			enemyCount++;
		}
	}
}


sf::Vector2f EnemyManager::randomSpawnPosition() {
    const sf::View& view = (*cameraPtr).getView();
	sf::Vector2f viewPos = view.getCenter();
	sf::Vector2f viewSize = view.getSize();
    sf::CircleShape placeholder(enemySize);
    placeholder.setOrigin({enemySize, enemySize});
	float marginSize = 50.f;

	float left = viewPos.x - viewSize.x * 0.5f - marginSize;
	float right = viewPos.x + viewSize.x * 0.5f + marginSize;
	float top = viewPos.y - viewSize.y * 0.5f - marginSize;
	float bottom = viewPos.y + viewSize.y * 0.5f + marginSize;

	int attempts = 0;
	while (attempts < 10) {
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
        // check if the enemy collides with any existing hitbox
        placeholder.setPosition(spawnPosition);
        // return if no collision
		if ( !CollisionManager::getInstance().checkAllCollision(placeholder) ) {
			return spawnPosition;
		}
		attempts++;
        std::cout << attempts << std::endl;
	}

	// If good position not found within attempts, return position farthest from town
	sf::Vector2f corners[4] = {
	{left, top},
	{right, top},
	{left, bottom},
	{right, bottom}
	};
    // town is always at 0,0
	sf::Vector2f townPos = {0.f, 0.f};

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

void EnemyManager::update(float dt, const BuildingManager& buildManager) {
    spawnEnemy(dt);

    for (auto& enemy : enemies) {
		enemy->update(dt, buildManager);
	}

	for (auto it = enemies.begin(); it != enemies.end(); ) {
		if (!(*it)->checkAlive()) {
            enemyCount--;
            EntityManager::getInstance().incrementScore();
			
			it = enemies.erase(it);
		}
		else {
			++it;
		}
	}
}

void EnemyManager::render(sf::RenderWindow& window) {
    for(auto& enemy : enemies) {
        enemy->render(window);
    }
}

void EnemyManager::reset() {
    enemies.clear();
    enemyCount = 0;
}

const std::vector<std::shared_ptr<Enemy>>& EnemyManager::getEnemyList() const {
    return enemies;
}

void EnemyManager::setSpawnMinMax(float min, float max) {
	if(min < 0.05f) minSpawnInterval = 0.05f;
	else minSpawnInterval = min;
	if(max < 0.15f) maxSpawnInterval = 0.15f;
	else maxSpawnInterval = max;
}

void EnemyManager::setMaxSpawn(int max) {
	if(max > 100) maxEnemies = 100;
	else maxEnemies = max;
}