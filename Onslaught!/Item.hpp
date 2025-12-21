#pragma once
#include "ItemData.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

// Instance of Item (on map, inventory, etc)
class Item {
private:
    ItemID id;

    int quantity = 1;
    float lifetime = 0.f;
    float movementSpeed = 150.f;
    sf::Vector2f position{};
    sf::Sprite sprite;

    bool expired = false;
    bool inPickupRange = false;
    float pickupRange = 60.f;


public:
    Item(ItemID id_, const sf::Vector2f& pos, int qty);

    void update(float dt, sf::Vector2f playerPos);

    void render(sf::RenderWindow& window) const;

    bool isStackable() const;

    int maxStack() const;

    bool isExpired() const;

    bool isPickable() const;

    const std::string& name() const;

    int getQuantity() const;

    void setQuantity(int newqty);

    ItemID getItemID() const;
};
