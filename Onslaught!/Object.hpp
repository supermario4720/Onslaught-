#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "ObjectID.hpp"
#include "Entity.hpp"
#include "Hitbox.hpp"

// 障害物、オブジェクトなどのクラス（木、石）
class Object : public Entity {
private:
    ObjectID objID;
    std::weak_ptr<Object> selfPtr;
public:
    Object(ObjectID id, sf::Vector2f position);
    ~Object();
    static std::shared_ptr<Object> create(ObjectID id, sf::Vector2f position);
    void initializeHitbox() override;
    void initializePtr(std::shared_ptr<Object> ptr);

    void update(float dt) override;

    void onCollision(float damage, sf::Vector2f damageOrigin) override;

    void onDeath() override;

    void render(sf::RenderWindow& window) override;

    const sf::Vector2f getPosition() const override;
};
