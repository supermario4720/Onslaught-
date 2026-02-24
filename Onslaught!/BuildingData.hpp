#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include "BuildingID.hpp"
#include "ItemID.hpp"

struct BuildingCost {
    ItemID id;
    int amount;
};

struct BuildingData {
    std::string name;
    std::string description;
    std::vector<BuildingCost> buildCost;
    sf::Vector2f size;

    float health;
};

class BuildingDatabase {
public: 
	static const BuildingData& get(BuildingID id);

private:
	static const std::unordered_map<BuildingID, BuildingData>& getMap();
};