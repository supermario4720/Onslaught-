#include "ItemTextures.hpp"


void ItemTextures::loadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    textures[name] = std::move(tex);
}

// Retrieve a texture
const sf::Texture& ItemTextures::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    throw std::runtime_error("Button texture not found: " + name);
}

void ItemTextures::loadAllTextures() {
    loadTexture("temp", "resources/buttonTextures/Button_Blue.png");
}