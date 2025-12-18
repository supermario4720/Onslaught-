#pragma once
#include "Entity.hpp"
#include "BuildingData.hpp"
#include "BuildingTextures.hpp"
#include "Hitbox.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class EnemyManager;

class Building : public Entity {
protected:
    std::weak_ptr<Building> selfPtr;
    std::shared_ptr<Hitbox> buildingHitbox;
    BuildingID buildingID;
    sf::Vector2f position;
    sf::Sprite sprite;

    int faction;
    bool expired;


public:
    Building(BuildingID id, sf::Vector2f pos, int _faction = 0);

    static std::shared_ptr<Building> create(BuildingID id, sf::Vector2f pos, int _faction = 0);

    void initializePtr(std::shared_ptr<Building> ptr);

    void initializeHitbox() override;

	void onCollision(float damage) override;

    void update(float dt) override;
    // overload function for usage in subclasses
    virtual void update(float dt, EnemyManager& enemyManager);

    void render(sf::RenderWindow& window) override;

    bool isExpired() const;


};