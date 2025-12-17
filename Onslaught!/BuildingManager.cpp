#include "Building.hpp"
#include "BuildingManager.hpp"
#include "InputManager.hpp"
#include "InventoryManager.hpp"
#include "CollisionManager.hpp"


#include <vector>
#include <iostream>


BuildingManager::BuildingManager()
: mousePos( {0.f, 0.f} ), isBuildMode(false), selectedBuilding(BuildingID::ArcherTower)
{
    const auto& data = BuildingDatabase::get(selectedBuilding);
    sf::Vector2f buildingSize = data.size;

    bounds.setSize( buildingSize );
    bounds.setOrigin( buildingSize/2.f );
    bounds.setFillColor( sf::Color(100, 255, 100, 155) );
}

// make it so that the function only needs to be called, and no params needed
    // building ID should be stored in the manager, with a select (update function updates the selected building on input)
void BuildingManager::createBuilding() {
    const auto& data = BuildingDatabase::get(selectedBuilding);
    sf::Vector2f buildingSize = data.size;

    std::shared_ptr<Building> newBuilding = std::make_shared<Building>(selectedBuilding, mousePos);
    buildings.push_back(newBuilding);
}

void BuildingManager::update(float dt, sf::Vector2f playerPos, InventoryManager& playerInventory, sf::RenderWindow& window) {
    InputManager& input = InputManager::getInstance();
    mousePos = input.getMousePosition(window);

    if(isBuildMode) {
        bool isOverlapped = CollisionManager::getInstance().checkAllCollision(bounds);
        bounds.setPosition(mousePos);
        if( isOverlapped ) {
            bounds.setFillColor( sf::Color(255, 100, 100, 155) );

            if( input.isKeyDown(sf::Keyboard::Key::T) ){
                createBuilding();
            }
        }
        else {
            bounds.setFillColor( sf::Color(100, 255, 100, 155) );
        }
    }

    for (auto& building : buildings) {
        building -> update(dt);
    }

    // remove expired items
    for (auto it = buildings.begin(); it != buildings.end(); ) {
        if ((*it)->isExpired()) {
            it = buildings.erase(it);
        }
        else {
            ++it;
        }
    }
}

void BuildingManager::cycleSelectedBuilding() {
    selectedBuilding = nextBuildingID(selectedBuilding);
    // update the build bounds
    const auto& data = BuildingDatabase::get(selectedBuilding);
    sf::Vector2f buildingSize = data.size;
    bounds.setSize( buildingSize );
    bounds.setOrigin( buildingSize/2.f );
}

void BuildingManager::render(sf::RenderWindow& window) {
    for (const auto& building : buildings) {
        building -> render(window);
    }
    if (isBuildMode) {
        window.draw(bounds);
    }
}

void BuildingManager::toggleBuildMode() {
    isBuildMode = !isBuildMode;
}

void BuildingManager::reset() {
    buildings.clear();
    isBuildMode = false;

    selectedBuilding = BuildingID::ArcherTower;
    
    const auto& data = BuildingDatabase::get(selectedBuilding);
    sf::Vector2f buildingSize = data.size;
    bounds.setSize( buildingSize );
    bounds.setOrigin( buildingSize/2.f );
    bounds.setFillColor( sf::Color(100, 255, 100, 155) );
}
