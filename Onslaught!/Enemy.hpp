#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Hitbox.hpp"
#include "AnimationController.hpp"

class EntityManager;

class Enemy : public Entity {
private:
	std::weak_ptr<Enemy> selfPtr;
	EntityManager& entityManager;
	sf::CircleShape enemy;
	// sf::Vector2f homePos;

	sf::Texture spriteSheet;
	sf::Sprite sprite;
	AnimationController animations;
	AnimationController::State targetState = AnimationController::State::IdleRight;

	float scale = 3.f;
	bool facingLeft = false;
	float spriteOffset = -15.f;

	std::shared_ptr<Hitbox> enemyHB;

	sf::Angle facing;
	sf::Vector2f movementVector;

	sf::Vector2f knockbackVector;
	float knockbackDuration;
	float currentKnockbackTime;

	float movementSpeed;
	float size;

	bool isAttacking;
	bool isTakingDamage;
	float attackDamage;

public:
	Enemy(sf::Vector2f initPos, float enemySize=30.f, float speed = 75.f );
	~Enemy();
	static std::shared_ptr<Enemy> create(sf::Vector2f initPos, float enemySize = 23.f, float speed = 60.f);
	void initializeHitbox() override;
	void initializePtr(std::shared_ptr<Enemy> ptr);

	//void update(float dt, sf::Vector2f playerPos);
	// instead of passing playerPos, read it from inside update -> make player be publically known
	void onCollision(float damage) override;

	void onDeath() override;

	void update(float dt) override;

	void render(sf::RenderWindow& window) override;

	sf::Vector2f getPosition();

	void updateAnimationState(sf::Vector2f moveVec);

	void calculateKnockback();
};