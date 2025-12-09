#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class ButtonTextureLoader {
public:
    static ButtonTextureLoader& getInstance() {
        static ButtonTextureLoader instance;
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

    ButtonTextureLoader() = default;
    ~ButtonTextureLoader();
    ButtonTextureLoader(const ButtonTextureLoader&) = delete;
    ButtonTextureLoader& operator=(const ButtonTextureLoader&) = delete;
};
