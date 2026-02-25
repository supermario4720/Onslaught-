#pragma once
#include <cstdint>
//　アイテムのID
//　別のファイルに入れることで拡張しやすい
enum class ItemID : uint16_t {
    None = 0,
    Wood,
    Stone,
    COUNT
};