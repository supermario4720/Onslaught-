#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "Entity.hpp"

class Hitbox;
//　拠点のクラス
class Town : public Entity {
private:
	std::weak_ptr<Town> selfPtr;
	sf::RectangleShape rect;

	const sf::Texture& normalTexture;
	const sf::Texture& damageTexture;
	const sf::Texture& destroyedTexture;

	float spriteOffset = 10.f;
	bool playerNear = false;
	float interactionRadius;

public:
	Town();

	~Town();
	
	// fully construct and return shared ptr of Town instance
	//　クラスのインスタンスを作り、ポインターを返す関数
	static std::shared_ptr<Town> createTown();

	void initializePtr(std::shared_ptr<Town>);

	void initializeHitbox() override;

	void onCollision(float damage, sf::Vector2f damageOrigin) override;

	bool wasDamaged() const;

	void update(float dt) override;

	void render(sf::RenderWindow& window) override;

	void checkPlayerNear(sf::Vector2f playerPos);

	sf::Vector2f getTownPosition() const;

	sf::Vector2f getTownSize() const;

	sf::FloatRect getBounds() const;

	bool isPlayerNear();

	void setColor(sf::Color color);
};