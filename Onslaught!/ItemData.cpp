#pragma once
#include "ItemData.hpp"

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

enum class ItemID : uint16_t {
    None = 0,
    Wood = 1,
    Stone = 2,
    Bone = 3
};

const ItemData& ItemDatabase::get(ItemID id) {
    return getMap().at(id);
}


const std::unordered_map<ItemID, ItemData>& ItemDatabase::getMap() {
    static const std::unordered_map<ItemID, ItemData> map = {
        { ItemID::Wood, {
            "Wood",
            "Basic material for construction.",
            "items/placeholder.png",
            true,
            99
        }},

        { ItemID::Stone, {
            "Mana Potion",
            "Basic material for construction.",
            "items/placeholder.png",
            true,
            99
        }},
        { ItemID::Bone, {
            "Iron Sword",
            "A basic iron sword.",
            "items/placeholder.png",
            true,
            99
        }},
    };
    return map;
}
