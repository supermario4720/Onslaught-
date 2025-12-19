#include "ObjectManager.hpp"
#include "ObjectData.hpp"
#include "CollisionManager.hpp"
#include "EntityManager.hpp"
#include "Camera.hpp"
#include <iostream>

ObjectManager::ObjectManager()
: objectCount(0), maxObjects(20)
{}

void ObjectManager::setCamera(Camera* ptr) {
    cameraPtr = ptr;
}


sf::Vector2f ObjectManager::randomSpawnPosition(ObjectID id) {
    const sf::View& view = (*cameraPtr).getView();
    sf::Vector2f viewPos = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f spawnPos;
    // setting corners for spawn positions
    float margin = 50.f;
    float farthestSpawnRange = 1000.f;
    float left = viewPos.x - viewSize.x * 0.5f - margin;
    float right = viewPos.x + viewSize.x * 0.5f + margin;
    float top = viewPos.y - viewSize.y * 0.5f - margin;
    float bottom = viewPos.y + viewSize.y * 0.5f + margin;

    // setting up placeholder shape to check if spot open
    sf::RectangleShape placeholder;
    const ObjectData& obj = ObjectDatabase::get(id);
    if(obj.rad == 0) {
        placeholder.setSize( obj.size );
        placeholder.setOrigin( obj.size/2.f);
    }
    else {
        placeholder.setSize( {obj.rad*2.f, obj.rad*2.f} );
        placeholder.setOrigin( {obj.rad, obj.rad} );
    }

    float innerR = viewSize.length()/2.f + margin;
    float outerR = innerR + farthestSpawnRange;

    // get random position
    int attempts = 0;
	while (attempts < 10) {
        float angle  = randf(0.f, 2.f * 3.14159265f);
        float radius = randf(innerR, outerR);

        spawnPos ={
            viewPos.x + std::cos(angle) * radius,
            viewPos.y + std::sin(angle) * radius
        };
		
        // check if the enemy collides with any existing hitbox
        placeholder.setPosition(spawnPos);
        // return if no collision
		if ( !CollisionManager::getInstance().checkAllCollision(placeholder) ) {
			return spawnPos;
		}
		attempts++;
        std::cout << attempts << std::endl;
    }

    // if failed to get position, return corner farthest from town
	sf::Vector2f corners[4] = {
	{left, top},
	{right, top},
	{left, bottom},
	{right, bottom}
	};
    // town is always at 0,0
	sf::Vector2f townPos = {0.f, 0.f};

	float maxDist = 0.f;
	sf::Vector2f bestCorner = corners[0];
	for (const auto& corner : corners) {
		float dx = corner.x - townPos.x;
		float dy = corner.y - townPos.y;
		float dist = std::sqrt(dx * dx + dy * dy);
		if (dist > maxDist) {
			maxDist = dist;
			bestCorner = corner;
		}
	}

	return bestCorner;
}

void ObjectManager::spawnRandomObject() {
    if (objectCount >= maxObjects) return;

    // pick a random ObjectID from [1, COUNT)
    int first = 1;
    int last  = static_cast<int>(ObjectID::COUNT) - 1;
    ObjectID objID = static_cast<ObjectID>(rand() % (last - first + 1) + first);

    sf::Vector2f pos = randomSpawnPosition(objID);

    auto obj = Object::create(objID, pos);
    obj->setEntityID(EntityManager::getInstance().getNextEntityID());
    objects.push_back(obj);
    ++objectCount;
}

void ObjectManager::update(float dt) {
    lifetime += dt;
    // dont spawn objects for the first 2 seconds of games
    if(!canSpawn && lifetime > 2.f) canSpawn = true;
    // keep spawning until we reach max
    while (canSpawn && (objectCount < maxObjects) ) {
        spawnRandomObject();
    }

    for (auto& obj : objects) {
        obj->update(dt);
    }

    // add itemManager here to manage the drops
    for (auto it = objects.begin(); it != objects.end(); ) {
        if (!(*it)->checkAlive()) {
            --objectCount;
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
}

void ObjectManager::render(sf::RenderWindow& window) {
    for (auto& obj : objects) {
        obj->render(window);
    }
}

void ObjectManager::reset() {
    objects.clear();
    objectCount = 0;
    lifetime = 0.f;
    canSpawn = false;
}
