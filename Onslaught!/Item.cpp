#include "Item.hpp"
#include "TextureManager.hpp"


Item::Item(ItemID _id, const sf::Vector2f& pos, int qty)
    : id(_id), quantity(qty), position(pos), 
    sprite(TextureManager::getInstance().getTexture( _id ))
{
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });


    sprite.setScale({ 20.f/bounds.size.x, 20.f/bounds.size.y });
    sprite.setPosition(position);
}

void Item::update(float dt, sf::Vector2f playerPos) {
    if (expired) return;

    lifetime += dt;
    if (lifetime > 60.f) {
        expired = true;
    }

    sf::Vector2f distance = playerPos - sprite.getPosition();
    sf::Vector2f movementVector = distance;
    if (movementVector.x != 0.f || movementVector.y != 0.f) {
        movementVector = movementVector.normalized() * movementSpeed;
    }
    float distlen = distance.length();

    if (distlen < 10.f) {
        inPickupRange = true;
    }
    else {
        inPickupRange = false;
    }

    if (distlen < pickupRange) {
        float relativeSpeed = (110.f - distlen)* (110.f - distlen) / 12100.f;
        if (relativeSpeed > 1.f) relativeSpeed = 1.f;
        sprite.move(movementVector * relativeSpeed* dt);
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

ItemID Item::getItemID() const {
    return id;
}

int Item::getQuantity() const {
    return quantity;
}

void Item::setQuantity(int newqty) {
    quantity = newqty;
    if(quantity <= 0) {
        expired = true;
    }
}