#include "UITextures.hpp"

UITextures::~UITextures() {
    textures.clear();
}

void UITextures::loadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    textures[name] = std::move(tex);
}

// Retrieve a texture
const sf::Texture& UITextures::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    throw std::runtime_error("Button texture not found: " + name);
}

void UITextures::loadAllTextures() {
    loadTexture("Button", "resources/buttonTextures/Button_Blue.png");
    loadTexture("LongButton", "resources/buttonTextures/Button_Blue_3Slides.png");

    loadTexture("ButtonHover", "resources/buttonTextures/Button_Hover.png");
    loadTexture("LongButtonHover", "resources/buttonTextures/Button_Hover_3Slides.png");

    loadTexture("ButtonPressed", "resources/buttonTextures/Button_Disable.png");
    loadTexture("LongButtonPressed", "resources/buttonTextures/Button_Disable_3Slides.png");
}

void UITextures::clearManager() {
	textures.clear();
}