#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class UITextures {
public:
    static UITextures& getInstance() {
        static UITextures instance;
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

    UITextures() = default;
    ~UITextures();
    UITextures(const UITextures&) = delete;
    UITextures& operator=(const UITextures&) = delete;
};
