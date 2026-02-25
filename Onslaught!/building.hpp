#pragma once
#include "Entity.hpp"
#include "BuildingData.hpp"
#include "TextureManager.hpp"
#include "Hitbox.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

class EnemyManager;

//　建物の基底クラス
class Building : public Entity {
protected:
    std::weak_ptr<Building> selfPtr;
    BuildingID buildingID;

    bool expired;

public:
    Building(BuildingID id, sf::Vector2f pos, int _faction = 0);

    static std::shared_ptr<Building> create(BuildingID id, sf::Vector2f pos, int _faction = 0);

    void initializePtr(std::shared_ptr<Building> ptr);

    void initializeHitbox() override;

	void onCollision(float damage, sf::Vector2f damageOrigin) override;

    void update(float dt) override;
    // overload function for usage in subclasses
    //子クラスように変更したUpdate（）
    virtual void update(float dt, EnemyManager& enemyManager);

    void render(sf::RenderWindow& window) override;

    bool isExpired() const;

    const sf::Vector2f getPosition() const override;

};