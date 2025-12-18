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
            "temp", //"items/~~~.png",
            false,
            1
        }},

        { ItemID::Wood, {
            "Wood",
            "Basic material for construction.",
            "temp",
            true,
            99
        }},

        { ItemID::Stone, {
            "Stone",
            "Basic material for construction.",
            "temp",
            true,
            99
        }},
        { ItemID::Bone, {
            "Bone",
            "Basic item drop from skeletons.",
            "temp",
            true,
            99
        }},
    };
    return map;
}
