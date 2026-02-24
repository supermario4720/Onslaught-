#include "TextureManager.hpp"
#include "ObjectData.hpp"

TextureManager::TextureManager() {
    if (!font.openFromFile("resources/arial.ttf")) {}
}

TextureManager::~TextureManager() {
    clearManager();
}

void TextureManager::loadTexture(const std::string& name, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    textures[name] = std::move(tex);
}
void TextureManager::loadTexture(const BuildingID id, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    buildingTextures[static_cast<std::size_t>(id)] = tex;
}
void TextureManager::loadTexture(const ObjectID id, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    objectTextures[static_cast<std::size_t>(id)] = tex;
}
void TextureManager::loadTexture(const ItemID id, const std::string& filepath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filepath)) {
        // Handle file load error here (e.g., log or throw)
        return;
    }
    itemTextures[static_cast<std::size_t>(id)] = tex;
}
// void TextureManager::loadTexture(const EnemyID id, const std::string& filepath) {
// }


const sf::Texture& TextureManager::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end())
        return it->second;
    throw std::runtime_error("Texture not found: " + name);
}
const sf::Texture& TextureManager::getTexture(const BuildingID id) const {
    auto index = static_cast<std::size_t>(id);
    return buildingTextures[index];
}
const sf::Texture& TextureManager::getTexture(const ObjectID id) const {
    auto index = static_cast<std::size_t>(id);
    return objectTextures[index];
}
const sf::Texture& TextureManager::getTexture(const ItemID id) const {
    auto index = static_cast<std::size_t>(id);
    return itemTextures[index];
}

const sf::Font& TextureManager::getFont() const {
    return font;
}



void TextureManager::loadAllTextures() {

    // Loading in MISC textures (Player, UI, other)
    loadTexture("temp", "resources/buttonTextures/Button_Blue.png");
    loadTexture("tempBuilding", "resources/buttonTextures/Button_Blue.png");
    loadTexture("Player", "resources/dwarfx1.png");
    loadTexture("Experience","resources/buttonTextures/Button_Disable.png" );

    loadTexture("Enemy", "resources/skeleton.png" );

    loadTexture("TownNormal", "resources/Castle_Blue.png" );
    loadTexture("TownHurt", "resources/Castle_Red.png" );
    loadTexture("TownDestroyed", "resources/Castle_Destroyed.png" );


    loadTexture("Button", "resources/buttonTextures/Button_Blue.png");
    loadTexture("LongButton", "resources/buttonTextures/Button_Blue_3Slides.png");
    loadTexture("ButtonHover", "resources/buttonTextures/Button_Hover.png");
    loadTexture("LongButtonHover", "resources/buttonTextures/Button_Hover_3Slides.png");
    loadTexture("ButtonPressed", "resources/buttonTextures/Button_Disable.png");
    loadTexture("LongButtonPressed", "resources/buttonTextures/Button_Disable_3Slides.png");
    loadTexture("Pointer", "resources/TownPointer.png");
    loadTexture("KeyboardButtons", "resources/buttonTextures/Keyboard_Letters_and_Symbols.png");
    loadTexture("KeyboardExtraButtons", "resources/buttonTextures/Keyboard_Extras.png");

    // Loading in building textures
    loadTexture(BuildingID::None, "resources/buttonTextures/Button_Blue.png");
    loadTexture(BuildingID::ArcherTower, "resources/Tower.png");
    loadTexture(BuildingID::Wall, "resources/WallSegment.png");

    // Loading in object textures
    loadTexture(ObjectID::None, "resources/buttonTextures/Button_Blue.png");
    loadTexture(ObjectID::Tree, "resources/Tree.png");
    loadTexture(ObjectID::Stone, "resources/Rock_Field.png");


    // Loading in item textures
    loadTexture(ItemID::None, "resources/buttonTextures/Button_Blue.png");
    loadTexture(ItemID::Wood, "resources/Wood_Item.png");
    loadTexture(ItemID::Stone, "resources/Rock_Item.png");
}

void TextureManager::clearManager() {
    textures.clear();
    buildingTextures.fill(sf::Texture());
    objectTextures.fill(sf::Texture());
    itemTextures.fill(sf::Texture());

    // enemyTextures.fill(sf::Texture());
}