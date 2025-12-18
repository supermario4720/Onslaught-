#pragma once
#include "Building.hpp"

class EnemyManager;

class ArcherTower : public Building {
public:
    ArcherTower(BuildingID id, sf::Vector2f pos, int faction = 0);
    ~ArcherTower();

    static std::shared_ptr<ArcherTower> create(BuildingID id, sf::Vector2f pos, int faction = 0);

    void initializePtr(std::shared_ptr<ArcherTower> ptr);

    void update(float dt, EnemyManager& enemyManager) override;
    void render(sf::RenderWindow& window) override;

private:
    float attackRange      = 250.f;
    float attackCooldown   = 0.8f;
    float timeSinceLastShot = 0.f;
    float arrowSpeed       = 400.f;
    float arrowDamage      = 10.f;

    struct Arrow {
        sf::RectangleShape shape;
        sf::Vector2f velocity;
        bool alive = true;
        float lifetime = 0.f;
        float maxLifetime = 2.f;
    };
    std::vector<Arrow> arrows;

    void tryShootClosestEnemy(EnemyManager& enemyManager);
    void updateArrows(float dt, EnemyManager& enemyManager);
};
