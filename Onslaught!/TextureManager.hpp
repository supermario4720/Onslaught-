#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include "BuildingID.hpp"
#include "ObjectID.hpp"
#include "ItemID.hpp"
// #include "EnemyID.hpp

#include <string>
#include <unordered_map>
#include <array>

//全てのテキスチャーをロードし、管理するためのクラス
class TextureManager {
public:
    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    // Load textures into the manager
    //テキスチャーをロードする関数
        //各IDで登録できるように、ポリモーフィズムを使用
    void loadTexture(const std::string& name, const std::string& filepath);
    void loadTexture(const BuildingID id, const std::string& filepath);
    void loadTexture(const ObjectID id, const std::string& filepath);
    void loadTexture(const ItemID id, const std::string& filepath);
    // void loadTexture(const EnemyID id, const std::string& filepath);

    // Retrieve a texture
    //テキスチャーを呼び出すための関数
        //各IDで登録できるように、ポリモーフィズムを使用
    const sf::Texture& getTexture(const std::string& name) const;
    const sf::Texture& getTexture(const BuildingID id) const;
    const sf::Texture& getTexture(const ObjectID id) const;
    const sf::Texture& getTexture(const ItemID id) const;
    // const sf::Texture& getTexture(const EnemyID id) const;

    const sf::Font& getFont() const;

    void loadAllTextures();

    void clearManager();

private:
    // includes UI, player, background, etc
    //様々なテキスチャーを保存するためのMap
    std::unordered_map<std::string, sf::Texture> textures;

    std::array<sf::Texture, static_cast<std::size_t>(BuildingID::COUNT)> buildingTextures;

    std::array<sf::Texture, static_cast<std::size_t>(ObjectID::COUNT)> objectTextures;

    std::array<sf::Texture, static_cast<std::size_t>(ItemID::COUNT)> itemTextures;

    // EnemyID::COUNT
    // std::array<sf::Texture, static_cast<std::size_t>(EnemyID::COUNT)> enemyTextures;

    TextureManager();
    ~TextureManager();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    sf::Font font;
};
