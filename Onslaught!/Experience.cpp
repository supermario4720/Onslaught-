#include "Experience.hpp"
#include "TextureManager.hpp"


Experience::Experience(const sf::Vector2f& pos, int qty)
    : quantity(qty), position(pos), 
    sprite(TextureManager::getInstance().getTexture( "Experience" ))
{
    auto bounds = sprite.getLocalBounds();
    sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    sprite.setScale({ 10.f/bounds.size.x, 10.f/bounds.size.y });
    sprite.setPosition(position);
}

void Experience::update(float dt, sf::Vector2f playerPos) {
    if (expired) return;

    lifetime += dt;
    if (lifetime > maxLifetime) {
        expired = true;
    }

    sf::Vector2f movementVector = playerPos - sprite.getPosition();
    float distlen = movementVector.length();
    if (movementVector.x != 0.f || movementVector.y != 0.f) {
        movementVector = movementVector.normalized() * movementSpeed;
    }

    if (distlen < 10.f) {
        inPickupRange = true;
    }
    else {
        inPickupRange = false;
    }

    if (distlen < pickupRange) {
        // make the speed increase as distance gets smaller (by ^2)
        float relativeSpeed = (110.f - distlen) * (110.f - distlen) / 12100.f;
        if (relativeSpeed > 1.f) relativeSpeed = 1.f;
        sprite.move(movementVector * relativeSpeed * dt);
    }
}

void Experience::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Experience::isExpired() const {
    return expired;
}

bool Experience::isPickable() const {
    return inPickupRange;
}

int Experience::getQuantity() const {
    return quantity;
}

void Experience::setQuantity(int newqty) {
    quantity = newqty;
    if(quantity <= 0) {
        expired = true;
    }
}