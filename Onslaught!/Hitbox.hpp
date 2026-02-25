#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <iostream>

class Entity;
//　当たり判定の基底クラス（普通の衝突に使用）
class Hitbox {
protected:
	// 0 for rect, 1 for circle
	int shapeType;
	sf::RectangleShape rectHB;
	sf::CircleShape circleHB;
	float lifetime;
	bool finished;
	bool isVisible;
	// 0 = player, 1 = enemy, 2 = object
	int faction;
	// Collision 確認時に使う
	int id;

public:
	std::weak_ptr<Entity> hitboxOwner;
	// rectangle hitbox constructor
	Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, int _faction = 0, bool _isVisible = false);
	// circle hitbox constructor
	Hitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, int _faction = 0, bool _isVisible = false);

	virtual ~Hitbox() = default;

	void setID(int _id);

	int getID();

	virtual void update(float dt);

	virtual void render(sf::RenderWindow& window) const;

	virtual bool isExpired() const;

	virtual int getType() const;

	virtual sf::Vector2f getPosition();

	// might not need virtual for these
	virtual sf::Vector2f getSize();

	virtual float getRadius();

	virtual const sf::RectangleShape& getRectHitbox();
	
	virtual const sf::CircleShape& getCircleHitbox();

	// update the hitbox, and return the modified movement vector
	virtual sf::Vector2f updateHitbox(sf::Vector2f movementVec, bool doCollision = false, float damage = 0);

	void changeVisibility(bool vis);
	// instead, get owner ptr and call collision from there
	virtual void onCollision(float damage, sf::Vector2f damageSource);

	virtual void setFaction(int _faction);

	virtual int getFaction();

	virtual std::weak_ptr<Entity> getOwner();
};



// hitbox for attacks (trigger onCollision events)
//　衝突しない当たり判定のクラス
class TriggerHitbox : public Hitbox {
protected:
	float damage;
	float speed;
	float attackRange;

public:
	// constructor -> position and size for hitbox size
	TriggerHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, float damage, float speed, float range, int _faction);
	TriggerHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, float r, float damage, float speed, float range, int _faction);

	virtual void updateTriggerHitbox(float deltaTime, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection);	
};

//　プレイヤーの攻撃の当たり判定クラス
class SwingHitbox : public TriggerHitbox {
private:
	// distance from player origin to hitbox origin
	float offsetRadius;
	float initialAngle;
	float currentAngle;

public:
	// constructor -> position and size for hitbox size
	SwingHitbox(std::weak_ptr<Entity> owner, sf::Vector2f position, sf::Vector2f size, float damage, float speed, float range, int _faction);

	void updateTriggerHitbox(float deltaTime, sf::Vector2f currentPlayerPos, sf::Angle currentPlayerDirection) override;

};