#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include "ItemID.hpp"

// FIX ITEM PATHS IN MAP

// ==================================
// Will split item data, itemID enum, and map it to make it easier to call
// Will handle Item Instances (e.g. drops on the map/field) with a separate
//	Item Instance class or struct
// 
// This way,we can manage items without uneccessary functions/variables, 
//	makes it easier to change item traits, etc
// 
// 
// ==================================


struct ItemData {
    std::string name;
    std::string description;
    bool stackable;
    int maxStack;
};

class ItemDatabase {
public: 
	static const ItemData& get(ItemID id);

private:
	static const std::unordered_map<ItemID, ItemData>& getMap();
};