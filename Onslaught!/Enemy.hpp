#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Hitbox.hpp"
#include "ItemID.hpp"
#include "AnimationController.hpp"

class EntityManager;
class BuildingManager;
//　敵のクラス
class Enemy : public Entity {
private:
	std::weak_ptr<Enemy> selfPtr;
	EntityManager& entityManager;

	sf::CircleShape enemy;

	float scale = 3.f;
	bool facingLeft = false;
	float spriteOffset = -15.f;

	float facing;
	sf::Vector2f movementVector;

	float knockbackDuration;
	float currentKnockbackTime;

	float movementSpeed;
	float size;
	float detectionRadius;

	bool isAttacking;
	bool isTakingDamage;
	float attackDamage;

	// Item drops
	int droppedExp = 5;

public:
	Enemy(sf::Vector2f initPos, float enemySize=30.f, float speed = 75.f );
	~Enemy();
	//　クラスのインスタンスを作り、ポインターを返す関数
	static std::shared_ptr<Enemy> create(sf::Vector2f initPos, float enemySize = 23.f, float speed = 60.f);
	void initializeHitbox() override;
	void initializePtr(std::shared_ptr<Enemy> ptr);

	void onCollision(float damage, sf::Vector2f damageOrigin) override;

	void onAttack() override;

	void onDeath() override;

	void update(float dt) override;
	virtual void update(float dt, const BuildingManager& buildManager);

	void updatePosition(sf::Vector2f movementVec, float dt) override;

	void render(sf::RenderWindow& window) override;

	const sf::Vector2f getPosition() const override;

	const std::weak_ptr<Hitbox> getHitbox() const;

	void updateAnimationState(sf::Vector2f moveVec);

	void calculateKnockback(sf::Vector2f damageOrigin);

	sf::Vector2f chooseTarget(const BuildingManager& buildManager);
};