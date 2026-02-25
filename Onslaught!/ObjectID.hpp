#pragma once
#include <cstdint>

//　オブジェクト・障害物のID
//　別のファイルに入れることで拡張しやすい
enum class ObjectID : std::uint16_t {
    None = 0,
    Tree,
    Stone,
    COUNT
};
