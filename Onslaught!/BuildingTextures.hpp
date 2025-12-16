#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

class BuildingTextures {
public:
    static BuildingTextures& getInstance() {
        static BuildingTextures instance;
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

    BuildingTextures() = default;
    BuildingTextures(const BuildingTextures&) = delete;
    BuildingTextures& operator=(const BuildingTextures&) = delete;
};
