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



const ItemData& ItemDatabase::get(ItemID id) {
    return getMap().at(id);
}


const std::unordered_map<ItemID, ItemData>& ItemDatabase::getMap() {
    static const std::unordered_map<ItemID, ItemData> map = {
        { ItemID::None, {
            "None",
            "No Description",
            "items/placeholder.png",
            false,
            1
        }},

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
            "Bone",
            "A basic item drop from skeletons.",
            "items/placeholder.png",
            true,
            99
        }},
    };
    return map;
}
