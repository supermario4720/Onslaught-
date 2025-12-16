#include "BuildingTextures.hpp"


void BuildingTextures::loadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    textures[name] = std::move(tex);
}

// Retrieve a texture
const sf::Texture& BuildingTextures::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    throw std::runtime_error("Button texture not found: " + name);
}

void BuildingTextures::loadAllTextures() {
    loadTexture("tempBuilding", "resources/buttonTextures/Button_Blue.png");
}

void BuildingTextures::clearManager() {
    textures.clear();
}