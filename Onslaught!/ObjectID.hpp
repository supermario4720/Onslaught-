#pragma once
#include <cstdint>

enum class ObjectID : std::uint16_t {
    None = 0,
    Tree,
    Stone,
    COUNT
};
