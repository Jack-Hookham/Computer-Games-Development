#pragma once

#include "Entity.h"
#include "InputManager.h"
#include "SpriteSheet.h"

//Player class
//***Controls***
//A - move left
//D - move right
//Space - jump

enum playerMoveState
{
	IDLE,
	RUN,
	IN_AIR,
	ATTACK
};

class Player : public Entity
{
public:
	Player();
	~Player();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
		const Texture textures[], const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, bool fixedRotation = true);

	void update(InputManager& inputManager);

	//Cull then add Player to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

private:
	playerMoveState mMoveState = IDLE;

	//Player has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;

	//Array of sprite sheets for the different states
	
	SpriteSheet mSpriteSheets[4];
	//Number of sprites in each sheet
	int numSprites[4] = { 10, 10, 10, 10 };

	//Animation timer
	float mAnimationTime = 0.0f;

	//State bool checks
	bool mInAir = true;
	bool mAttacking = false;
};

