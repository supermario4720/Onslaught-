#include "ObjectData.hpp"

const ObjectData& ObjectDatabase::get(ObjectID id) {
    return data[static_cast<std::size_t>(id)];
}


const std::array<ObjectData,
    static_cast<std::size_t>(ObjectID::COUNT)> ObjectDatabase::data = {{
        { 
            ObjectID::None, 
            "No description",
            {0.f, 0.f},
            0.f,
            ItemID::None,
            0,
            1,
            1.f
        },
        { 
            ObjectID::Tree, 
            "Basic object and source of wood",
            {0.f, 0.f},
            30.f,
            ItemID::Wood,
            1,
            5,
            30.f
        },
        { 
            ObjectID::Stone, 
            "Basic object and source of stone",
            {0.f, 0.f},
            30.f,
            ItemID::Stone,
            1,
            5,
            30.f
        }
}};
