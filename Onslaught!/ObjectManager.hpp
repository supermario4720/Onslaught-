#pragma once
#include "Object.hpp"
#include "ObjectID.hpp"
#include <vector>

class Camera;

class ObjectManager {
public:
    ObjectManager();

    void setCamera(Camera* ptr);

    void update(float dt);
    void render(sf::RenderWindow& window);
    void reset();

    void spawnRandomObject();
    sf::Vector2f randomSpawnPosition(ObjectID id);

private:
    std::vector<std::shared_ptr<Object>> objects;
    Camera* cameraPtr = nullptr;

    int objectCount = 0;
    int maxObjects = 20;
    float lifetime = 0.f;
    bool canSpawn = false;
};
