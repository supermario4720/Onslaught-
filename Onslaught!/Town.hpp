#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "Entity.hpp"


class Hitbox;

class Town : public Entity {
private:
	std::weak_ptr<Town> selfPtr;
	sf::RectangleShape rect;

	sf::Texture normalTexture;
	sf::Texture damageTexture;
	sf::Texture destroyedTexture;
	sf::Sprite sprite;

	float spriteOffset = 10.f;

	std::shared_ptr<Hitbox> townHB;

	bool playerNear = false;

	float interactionRadius;
	

public:
	Town();

	~Town();
	
	// fully construct and return shared ptr of Town instance
	static std::shared_ptr<Town> createTown();

	void initializePtr(std::shared_ptr<Town>);

	void initializeHitbox() override;

	void onCollision(float damage) override;

	void update(float dt) override;

	void render(sf::RenderWindow& window) override;

	void checkPlayerNear(sf::Vector2f playerPos);

	sf::Vector2f getTownPosition() const;

	sf::Vector2f getTownSize() const;

	sf::FloatRect getBounds() const;

	bool isPlayerNear();

	void setColor(sf::Color color);
};