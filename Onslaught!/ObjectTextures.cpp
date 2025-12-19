#include "ObjectTextures.hpp"
#include "ObjectData.hpp"


void ObjectTextures::loadTexture(const ObjectID id, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    textures[static_cast<std::size_t>(id)] = tex;
}

// Retrieve a texture
const sf::Texture& ObjectTextures::getTexture(const ObjectID id) const {
    //auto index = static_cast<std::size_t>(id);
    // temporarily only return the 0th array value for textures
    auto index = static_cast<std::size_t>(ObjectID::None);

    return textures[index];

}

void ObjectTextures::loadAllTextures() {
    loadTexture(ObjectID::None, "resources/buttonTextures/Button_Blue.png");
}

void ObjectTextures::clearManager() {
    textures.fill(sf::Texture());
}