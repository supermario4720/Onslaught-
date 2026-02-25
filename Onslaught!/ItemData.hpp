#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include "ItemID.hpp"

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