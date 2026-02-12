#include "ArcherTower.hpp"
#include "BuildingData.hpp"
#include "EnemyManager.hpp"
#include "CollisionManager.hpp"
#include "Enemy.hpp"
#include <SFML/System/Angle.hpp>

ArcherTower::ArcherTower(BuildingID id, sf::Vector2f pos, int faction)
    : Building(id, pos, faction)
{
}

ArcherTower::~ArcherTower(){
    arrows.clear();
}

std::shared_ptr<ArcherTower> ArcherTower::create(BuildingID id, sf::Vector2f pos, int faction)
{
    std::shared_ptr<ArcherTower> tower = std::make_shared<ArcherTower>(id, pos, faction);
    tower->initializePtr(tower);
    tower->initializeHitbox();
    return tower;
}

void ArcherTower::initializePtr(std::shared_ptr<ArcherTower> ptr) {
    selfPtr = ptr;
    Building::initializePtr(ptr);
}

void ArcherTower::update(float dt, EnemyManager& enemyManager) {
    Building::update(dt);          // keep base behavior
    timeSinceLastShot += dt;

    tryShootClosestEnemy(enemyManager);
    updateArrows(dt, enemyManager);
}

void ArcherTower::render(sf::RenderWindow& window) {
    window.draw(sprite);
    entityHitbox->render(window);
    for (auto& arrow : arrows) {
        if (arrow.alive)
            window.draw(arrow.shape);
    }
}


void ArcherTower::tryShootClosestEnemy(EnemyManager& enemyManager) {
    // return if still on cooldown
    if (timeSinceLastShot < attackCooldown) return;

    sf::Vector2f towerPos = sprite.getPosition();

    std::shared_ptr<Enemy> closest = nullptr;
    float closestDistSqr = attackRange * attackRange;

    // get the list of enemies from enemy manager
    const auto& enemies = enemyManager.getEnemyList(); 

    // check closest enemy in range
    for (const auto& enemy : enemies) {
        if (!enemy || !enemy->checkAlive())
            continue;

        sf::Vector2f enemyPos = enemy->getPosition();
        float dx = enemyPos.x - towerPos.x;
        float dy = enemyPos.y - towerPos.y;
        float distSq = dx*dx + dy*dy;

        if (distSq <= closestDistSqr) {
            closestDistSqr = distSq;
            closest = enemy;
        }
    }
    // return if no enemies in range
    if (!closest)
        return;

    // if enemy in range, reset cooldown and shoot arrow
    timeSinceLastShot = 0.f;

    sf::Vector2f targetPos = closest->getPosition();
    sf::Vector2f dir = targetPos - towerPos;
    if (dir.x != 0 || dir.y != 0) dir = dir.normalized();
    else return;

    Arrow arrow;
    arrow.shape.setSize({10.f, 3.f});
    arrow.shape.setOrigin({5.f, 1.5f});
    arrow.shape.setPosition(towerPos);
    arrow.shape.setFillColor(sf::Color::Black);

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    arrow.shape.setRotation(sf::degrees(angleDeg));

    arrow.velocity = dir * arrowSpeed;
    arrow.alive = true;

    arrows.push_back(arrow);
}


void ArcherTower::updateArrows(float dt, EnemyManager& enemyManager) {
    auto& enemies = enemyManager.getEnemyList();

    for (auto& arrow : arrows) {
        if (!arrow.alive) continue;

        sf::Vector2f pos = arrow.shape.getPosition();
        pos += arrow.velocity * dt;
        arrow.shape.setPosition(pos);

        bool hit = false;

        for (const auto& enemy : enemies) {
            if (!enemy || !enemy->checkAlive())
                continue;

            const auto& enemyHB = enemy->getHitbox().lock();

            if ( CollisionManager::checkCircleRectCollision( enemyHB->getCircleHitbox(), arrow.shape ) ) {
                enemy->onCollision(arrowDamage, arrow.shape.getPosition());
                hit = true;
                break;
            }
        }

        if (hit) {
            arrow.alive = false;
        }
        arrow.lifetime += dt;
        if(arrow.lifetime > arrow.maxLifetime) arrow.alive = false;
    }

    arrows.erase(
        std::remove_if(arrows.begin(), arrows.end(),
                       [](const Arrow& a){ return !a.alive; }),
        arrows.end()
    );
}
