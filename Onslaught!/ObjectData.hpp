#pragma once
#include "ObjectID.hpp"
#include "ItemID.hpp"
#include <string>
#include <array>
#include <SFML/System/Vector2.hpp>

//　障害物・オブジェクトのデータ設計
struct ObjectData {
    ObjectID id;
    std::string description;
    // if rect shape, rad = 0.f;
    sf::Vector2f size;
    float rad;

    ItemID dropItem;
    int minDropQty;
    int maxDropQty;
    float maxHealth;
};
//　障害物・オブジェクトのデータベース（どこからでも呼べる）
class ObjectDatabase {
public: 
	static const ObjectData& get(ObjectID id);

private:
    static const std::array<ObjectData, static_cast<std::size_t>(ObjectID::COUNT)> data;

};