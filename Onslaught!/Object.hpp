#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Hitbox.hpp"


class Object : public Entity {
private:
    std::weak_ptr<Object> selfPtr;

    sf::RectangleShape objectShape;
    std::shared_ptr<Hitbox> objectHB;
public:
    Object(sf::Vector2f position, sf::Vector2f size);
    ~Object();
    static std::shared_ptr<Object> create(sf::Vector2f position, sf::Vector2f size);
    void initializeHitbox() override;
    void initializePtr(std::shared_ptr<Object> ptr);

    void update(float dt) override;

    void onCollision(float damage) override;

    void render(sf::RenderWindow& window) override;

    sf::RectangleShape getShape() const;

};


class ObjectCirc : public Entity {
private:
    std::weak_ptr<ObjectCirc> selfPtr;

    sf::CircleShape objectShape;
    std::shared_ptr<Hitbox> objectHB;
public:
    ObjectCirc(sf::Vector2f position, float rad);
    ~ObjectCirc();
    static std::shared_ptr<ObjectCirc> create(sf::Vector2f position, float rad);
    void initializeHitbox() override;
    void initializePtr(std::shared_ptr<ObjectCirc> ptr);

    void update(float dt) override;

    void onCollision(float damage) override;

    void render(sf::RenderWindow& window) override;

    sf::CircleShape getShape() const;

};