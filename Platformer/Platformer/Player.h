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
	JUMP, 
	IN_AIR,
	ATTACK,
	JUMP_ATTACK,
	NUM_STATES
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
	playerMoveState mState = IDLE;

	//Player has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;
	
	//IDLE, RUN, JUMP, IN_AIR, ATTACK, JUMP_ATTACK
	//Array of sprite sheets for the different states
	SpriteSheet mSpriteSheets[NUM_STATES];

	//Number of sprites for each state
	int mNumSprites[NUM_STATES] = { 10, 10, 3, 1, 10, 10};

	//Rows and columns in the sprite sheet
	glm::ivec2 mSheetDimensions[NUM_STATES] = { glm::ivec2(4, 3), glm::ivec2(5, 2), glm::ivec2(5, 2), 
		glm::ivec2(5, 2), glm::ivec2(3, 4), glm::ivec2(4, 3) };

	//Multipliers to adjust the sprite position and dimension based on the state (new dimensions/base dimensions)
	glm::vec2 mStateMultipliers[NUM_STATES] = { glm::vec2(1.0f), glm::vec2(1.56f, 1.04f),  glm::vec2(1.56f, 1.1f),
		glm::vec2(1.56f, 1.1f), glm::vec2(2.31f, 1.13f), glm::vec2(2.17f, 1.18f) };

	//Animation timer
	float mAnimationTimer = 0.0f;

	int mDirection = 1;

	//State bool checks
	bool mInAir = true;
	bool mAttacking = false;
	bool mJumping = false;

	float mJumpTimer = 0.0f;
	float mAttackTimer = 0.0f;
};

