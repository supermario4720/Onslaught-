#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

// Instance of exp on field
class Experience {
private:
    int quantity;
    float lifetime = 0.f;
    float maxLifetime = 30.f;
    float movementSpeed = 200.f;
    sf::Vector2f position;
    sf::Sprite sprite;

    bool expired = false;
    bool inPickupRange = false;
    float pickupRange = 60.f;


public:
    Experience(const sf::Vector2f& pos, int qty);

    void update(float dt, sf::Vector2f playerPos);

    void render(sf::RenderWindow& window) const;

    bool isExpired() const;

    bool isPickable() const;

    int getQuantity() const;

    void setQuantity(int newqty);
};
