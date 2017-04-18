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

	//void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
	//	const GLTexture& texture, const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f });

	void input(InputManager& inputManager);

	const Entity& getBox() const { return mCollisionBox; }

private:
	playerMoveState mMoveState;

	Entity mCollisionBox;
};

