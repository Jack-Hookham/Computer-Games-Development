#pragma once

#include "Entity.h"
#include "InputManager.h"

enum playerMoveState
{
	STANDING,
	RUNNING,
	PUNCHING,
	IN_AIR,
	AIR_ATTACK
};

class Player : public Entity
{
public:
	Player();
	~Player();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
		const Texture& texture, const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, bool fixedRotation = true);

	void input(InputManager& inputManager);

	//const Entity& getBox() const { return mCollisionBox; }

	//Cull and add player to spritebatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

private:
	playerMoveState mMoveState;

	b2Fixture* mFixtures[3];

	//Entity mCollisionBox;

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;
};

