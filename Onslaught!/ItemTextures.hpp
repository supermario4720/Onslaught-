#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

class ItemTextures {
public:
    static ItemTextures& getInstance() {
        static ItemTextures instance;
        return instance;
    }

    // Load textures into the manager
    void loadTexture(const std::string& name, const std::string& filepath);

    // Retrieve a texture
    const sf::Texture& getTexture(const std::string& name) const;

    void loadAllTextures();

    void clearManager();

private:
    std::unordered_map<std::string, sf::Texture> textures;

    ItemTextures() = default;
    ItemTextures(const ItemTextures&) = delete;
    ItemTextures& operator=(const ItemTextures&) = delete;
};
