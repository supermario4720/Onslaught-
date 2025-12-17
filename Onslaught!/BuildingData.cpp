#include "BuildingData.hpp"


const BuildingData& BuildingDatabase::get(BuildingID id) {
    return getMap().at(id);
}


const std::unordered_map<BuildingID, BuildingData>& BuildingDatabase::getMap() {
    static const std::unordered_map<BuildingID, BuildingData> map = {
        { BuildingID::None, {
            "None",
            "No Description",
            "tempBuilding", // "building/textureName",
            {},
            {1.f, 1.f},
            1.f
        }},

        { BuildingID::ArcherTower, {
            "Archer Tower",
            "Basic defensive tower.",
            "tempBuilding",
            {
                {ItemID::Wood, 5},
                {ItemID::Stone, 5}
            },
            {80.f, 80.f},
            200.f
        }},

        { BuildingID::Wall, {
            "Wall",
            "Basic defensive wall",
            "tempBuilding",
            {
                {ItemID::Wood, 5}
            },
            {20.f, 20.f},
            500.f
        }},
    };
    return map;
}