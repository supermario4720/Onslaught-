#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "ObjectID.hpp"
#include <string>
#include <array>

class ObjectTextures {
public:
    static ObjectTextures& getInstance() {
        static ObjectTextures instance;
        return instance;
    }

    // Load textures into the manager
    void loadTexture(const ObjectID id, const std::string& filepath);

    // Retrieve a texture
    const sf::Texture& getTexture(const ObjectID id) const;

    void loadAllTextures();

    void clearManager();

private:
    std::array<sf::Texture, static_cast<std::size_t>(ObjectID::COUNT)> textures;

    ObjectTextures() = default;
    ObjectTextures(const ObjectTextures&) = delete;
    ObjectTextures& operator=(const ObjectTextures&) = delete;
};
