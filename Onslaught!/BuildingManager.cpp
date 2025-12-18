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

    std::shared_ptr<Building> newBuilding = Building::create(selectedBuilding, mousePos);
    buildings.push_back(newBuilding);
}

void BuildingManager::update(float dt, sf::Vector2f playerPos, InventoryManager& playerInventory, sf::RenderWindow& window) {
    InputManager& input = InputManager::getInstance();
    mousePos = input.getMouseWorldPosition(window);

    if(isBuildMode) {
        bool isOverlapped = CollisionManager::getInstance().checkAllCollision(bounds);
        bounds.setPosition(mousePos);

        if( input.isKeyPressed(sf::Keyboard::Key::R) ) {
            cycleSelectedBuilding();
        }

        if( isOverlapped ) {
            bounds.setFillColor( sf::Color(255, 100, 100, 155) );
        }
        else {
            bounds.setFillColor( sf::Color(100, 255, 100, 155) );
            if( input.isKeyPressed(sf::Keyboard::Key::Space) || 
                input.isKeyPressed(sf::Keyboard::Key::Enter) ||
                input.isMousePressed(sf::Mouse::Button::Left)
            ){
                bool contained = false;
                if ( (contained = playerInventory.checkItemsForBuilding(selectedBuilding)) ) {
                    createBuilding();
                }
                std::cout << "Are items sufficient: " << contained << std::endl;
            }
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

void BuildingManager::setBuildMode(bool mode) {
    isBuildMode = mode;
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
