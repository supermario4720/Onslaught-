#pragma once
#include "BuildingID.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <iostream>

class InventoryManager;
class EnemyManager;
class Building;

class BuildingManager {
public:
    BuildingManager();

    // make it so that the function only needs to be called, and no params needed
        // building ID should be stored in the manager, with a select (update function updates the selected building on input)
    void createBuilding();

    void update(float dt, sf::Vector2f playerPos, InventoryManager& playerInventory, EnemyManager& enemyManager, sf::RenderWindow& window);

    void cycleSelectedBuilding();

    void render(sf::RenderWindow& window);

    void setBuildMode(bool mode);

    void reset();

    const std::vector<std::shared_ptr<Building>>& getBuildingList() const;

private:
    std::vector<std::shared_ptr<Building>> buildings;
    sf::Vector2f mousePos;
    sf::RectangleShape bounds;
    BuildingID selectedBuilding;

    bool isBuildMode;
};
