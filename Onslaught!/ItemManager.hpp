#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "ItemID.hpp"

class Item;
class InventoryManager;

class ItemManager {
public:
    void spawn(ItemID id, const sf::Vector2f& pos, int qty);

    void update(float dt, sf::Vector2f& playerPos, InventoryManager& playerInventory);

    void render(sf::RenderWindow& window);

    void reset();

private:
    std::vector<std::shared_ptr<Item>> items;
};
