#pragma once
#include "Item.hpp"
#include "ItemData.hpp"
#include "BuildingData.hpp"
#include "TextureManager.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>

//　インベントリーのスロット・枠
struct InventorySlot {
    ItemID id = ItemID::None;
    int quantity = 0;
    bool isFull = false;

    // for UI drawing
    sf::RectangleShape emptySlot;
    sf::Sprite sprite;
    sf::Text qtyText;
    InventorySlot(): sprite(TextureManager::getInstance().getTexture("temp")), qtyText(TextureManager::getInstance().getFont()) {};
};

//インベントリーを管理するためのクラス
class InventoryManager {
public:
    InventoryManager(int maxSlots = 15);

    int addItem(ItemID id, int amount);

    void reset();

    void printItems() const;

    bool checkItemsForBuilding(BuildingID id);

    int getItemCount(ItemID id) const;

    void removeItems(ItemID id, int amount);

    const std::vector<InventorySlot>& getInventory() const;

    void updateSlotTextures(float screenW, float screenH);

private:
    bool hasEmptySlot() const;
    bool isCompletelyFull() const;

    int maxInventorySlots;
    bool isInventoryFull;
    std::vector<InventorySlot> inventory;
};
