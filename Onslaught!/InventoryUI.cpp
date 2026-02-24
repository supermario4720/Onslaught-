#include <SFML/Graphics.hpp>
#include "InventoryUI.hpp"
#include "InventoryManager.hpp"
#include "InputManager.hpp"
#include "GameStateManager.hpp"
#include <iostream>

InventoryUI::InventoryUI(sf::Font& font, const sf::RenderWindow& window)
    : inventoryText(font), resumeButton({ 50.f, 50.f }, { 0.f, 0.f }, " X ", font)
{
    screenWidth = (float)window.getSize().x;
    screenHeight = (float)window.getSize().y;

    // the background dimmed out
    background.setSize({ screenWidth, screenHeight });
    background.setFillColor(sf::Color(0, 0, 0, 50));
    background.setPosition( {0, 0} );

    // Inventory UI background / base
    inventoryBase.setSize({ screenWidth*3.f/4.f, screenHeight*3.f/4.f });
    inventoryBase.setFillColor(sf::Color(30, 30, 30, 220));
    inventoryBase.setOrigin({ screenWidth*3.f/8.f, screenHeight*3.f/8.f });
    inventoryBase.setPosition( { screenWidth/2.f, screenHeight/2.f } );

    inventoryText = sf::Text(font);
    inventoryText.setCharacterSize(24);
    inventoryText.setString("Inventory");
    inventoryText.setFillColor(sf::Color(255, 255, 255, 200));
    auto bounds = inventoryText.getLocalBounds();
    inventoryText.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
    inventoryText.setPosition( { screenWidth/5.f, screenHeight/7.f } );

    resumeButton.setPosition({ screenWidth*7.f/8.f - 20.f, screenHeight/8.f + 20.f });  // Top-right
    resumeButton.setCharSize(32);

}

InventoryUI::~InventoryUI() {
}

void InventoryUI::update(float dt, sf::RenderWindow& window) {
    //sf::Vector2f screenSize = sf::Vector2f(window.getSize());
    auto& input = InputManager::getInstance();
    auto& state = GameStateManager::getInstance();
    
    bool resumeHover = resumeButton.isMouseOver(window);
    resumeButton.setHover(resumeHover);

    if (resumeButton.isClicked(window)) {
        inventoryOpen = false;
    }
}

void InventoryUI::render(sf::RenderWindow& window, const InventoryManager& inventoryManager) {
    window.draw(background);
    window.draw(inventoryBase);
    window.draw(inventoryText);
    resumeButton.render(window);
    
    const std::vector<InventorySlot>& inventory = inventoryManager.getInventory();
    for(auto slot : inventory) {
        if(slot.id == ItemID::None) window.draw(slot.emptySlot);
        else {
            window.draw(slot.sprite);
            window.draw(slot.qtyText);
        }
        
    }
}

void InventoryUI::setVisibility(bool open) {
    inventoryOpen = open;
}

bool InventoryUI::isInventoryOpen() const {
    return inventoryOpen;
}