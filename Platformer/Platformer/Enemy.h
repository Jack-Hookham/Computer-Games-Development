#pragma once

#include "EntityBox2D.h"
#include "Marker.h"
#include "Player.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "AudioManager.h"

enum EnemyAnimState
{
	ENEMY_IDLE,
	ENEMY_RUN,
	ENEMY_JUMP,
	ENEMY_IN_AIR,
	ENEMY_ATTACK,
	ENEMY_JUMP_ATTACK,
	ENEMY_NUM_STATES
};

enum EnemySound
{
	ENEMY_JUMP_SOUND,
	ENEMY_ATTACK_SOUND,
	ENEMY_NUM_SOUNDS
};

class Enemy : public EntityBox2D
{
public:
	Enemy();
	~Enemy();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position,
		const glm::vec2& dimensions, const Colour& colour, const Texture textures[],
		const SoundEffect sounds[], int id, const bool fixedRotation = true);

	void update(Player* player, std::vector<Marker*>& markerEntities);

	//Animate, cull then add Enemy to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const int getHealth() const { return mHealth; }
	inline const void setHealth(const int health) { mHealth = health; }

private:

	int mID;

	EnemyAnimState mAnimState = ENEMY_IDLE;

	//Enemy has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;

	//IDLE, RUN, JUMP, IN_AIR, ATTACK, JUMP_ATTACK
	//Array of sprite sheets for the different states
	SpriteSheet mSpriteSheets[ENEMY_NUM_STATES];

	//Number of sprites for each state
	int mNumSprites[ENEMY_NUM_STATES] = { 10, 10, 3, 1, 10, 10 };//Rows and columns in the sprite sheet
	glm::ivec2 mSheetDimensions[ENEMY_NUM_STATES] = { glm::ivec2(4, 3), glm::ivec2(5, 2), glm::ivec2(5, 2),
		glm::ivec2(5, 2), glm::ivec2(3, 4), glm::ivec2(4, 3) };

	//Multipliers to adjust the sprite position and dimension based on the state (new dimensions/base dimensions)
	glm::vec2 mStateMultipliers[ENEMY_NUM_STATES] = { glm::vec2(1.0f), glm::vec2(1.56f, 1.04f),  glm::vec2(1.56f, 1.1f),
		glm::vec2(1.56f, 1.1f), glm::vec2(2.31f, 1.13f), glm::vec2(2.17f, 1.18f) };

	//Animation timer
	float mAnimationTimer = 0.0f;

	int mSpriteDirection = 1;
	int mMoveDirection = 1;

	//State bool checks
	bool mInAir = true;
	bool mAttacking = false;
	bool mJumping = false;
	bool mSearching = true;

	float mJumpTimer = 0.0f;
	float mAttackTimer = 0.0f;

	const int DIRECTION_TIMER_CAP = 100;
	int mDirectionTimer;

	const float AGGRO_RANGE = 20.0f;

	int mHealth = 100;

	SoundEffect mSounds[ENEMY_NUM_SOUNDS];
};

