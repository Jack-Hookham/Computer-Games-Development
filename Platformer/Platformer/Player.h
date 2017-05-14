#pragma once

#include "EntityBox2D.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "AudioManager.h"

/*
* Player class
* ***Controls***
* ***Keyboard***
* A - move left
* D - move right
* W - jump
* Space - attack
* 
* ***Controller***
* A - move left
* D - move right
* W - jump
* Space - attack
*/

//Enum used to play the correct animation based on the current state
enum PlayerAnimState
{
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_JUMP, 
	PLAYER_IN_AIR,
	PLAYER_ATTACK,
	PLAYER_JUMP_ATTACK,
	PLAYER_NUM_STATES
};

enum PlayerSound
{
	PLAYER_JUMP_SOUND,
	PLAYER_ATTACK_SOUND,
	PLAYER_NUM_SOUNDS
};

class Player : public EntityBox2D
{
public:
	Player();
	~Player();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position, 
		const glm::vec2& dimensions, const Colour& colour, const Texture textures[], 
		const SoundEffect sounds[], const bool fixedRotation = true);

	void input(InputManager& inputManager);

	//Animate, cull then add Player to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const bool getAttacking() const { return mAttacking; }
	inline const int getHealth() const { return mHealth; }
	inline const int getDirection() const { return mDirection; }

	inline const void setHealth(const int health) { mHealth = health; }

private:

	PlayerAnimState mAnimState = PLAYER_IDLE;

	//Player has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;
	
	//PLAYER_IDLE, PLAYER_RUN, PLAYER_JUMP, PLAYER_IN_AIR, PLAYER_ATTACK, PLAYER_JUMP_PLAYER_ATTACK
	//Array of sprite sheets for the different states
	SpriteSheet mSpriteSheets[PLAYER_NUM_STATES];

	//Number of sprites for each state
	int mNumSprites[PLAYER_NUM_STATES] = { 10, 10, 3, 1, 10, 10};

	//Rows and columns in the sprite sheet
	glm::ivec2 mSheetDimensions[PLAYER_NUM_STATES] = { glm::ivec2(4, 3), glm::ivec2(5, 2), glm::ivec2(5, 2), 
		glm::ivec2(5, 2), glm::ivec2(3, 4), glm::ivec2(4, 3) };

	//Multipliers to adjust the sprite position and dimension based on the state (new dimensions/base dimensions)
	glm::vec2 mStateMultipliers[PLAYER_NUM_STATES] = { glm::vec2(1.0f), glm::vec2(1.56f, 1.04f),  glm::vec2(1.56f, 1.1f),
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

	int mHealth = 100;

	SoundEffect mSounds[PLAYER_NUM_SOUNDS];
};

