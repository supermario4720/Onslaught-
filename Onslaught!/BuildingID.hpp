#pragma once
#include <cstdint>

//　建物のID
//　別のファイルに入れることで拡張しやすい
enum class BuildingID : uint16_t {
    None = 0,
    ArcherTower,
    Wall,
    COUNT
};

// 建物のIDをループするためのコード
inline BuildingID nextBuildingID(BuildingID current) {
    using underlying = std::underlying_type_t<BuildingID>;
    underlying value = static_cast<underlying>(current);
    underlying max   = static_cast<underlying>(BuildingID::COUNT) - 1;

    if (value >= max) {
        return static_cast<BuildingID>(1); // skip None
    }
    return static_cast<BuildingID>(value + 1);
}
