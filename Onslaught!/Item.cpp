#pragma once
#include "Item.hpp"
#include "ItemTextures.hpp"


Item::Item(ItemID _id, const sf::Vector2f& pos, int qty)
    : id(_id), quantity(qty), position(pos), sprite(ItemTextures::getInstance().getTexture("temp"))
{
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    sprite.setScale({ 20.f, 20.f });
    sprite.setPosition(position);
}

void Item::update(float dt, sf::Vector2f playerPos) {
    if (expired) return;

    lifetime += dt;
    if (lifetime > 60.f) {
        expired = true;
    }

    sf::Vector2f distance = playerPos - sprite.getPosition();

    if (distance.length() < 10.f) {
        inPickupRange = true;
    }

    if (distance.length() < 100.f) {
        sprite.move(distance * dt * 5.f);
    }
}

void Item::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Item::isStackable() const {
    return ItemDatabase::get(id).stackable;
}

int Item::maxStack() const {
    return ItemDatabase::get(id).maxStack;
}

bool Item::isExpired() const {
    return expired;
}

bool Item::isPickable() const {
    return inPickupRange;
}

const std::string& Item::name() const {
    return ItemDatabase::get(id).name;
}
