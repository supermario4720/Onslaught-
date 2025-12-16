#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "BuildingID.hpp"
#include "ItemID.hpp"

struct BuildingCost {
    ItemID id;
    int amount;
};

struct BuildingData {
    std::string name;
    std::string description;
    std::string texture;
    std::vector<BuildingCost> buildCost;

    float health;
};

class BuildingDatabase {
public: 
	static const BuildingData& get(BuildingID id);

private:
	static const std::unordered_map<BuildingID, BuildingData>& getMap();
};