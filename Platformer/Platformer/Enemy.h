#pragma once

#include <random>

#include "EntityBox2D.h"
#include "Marker.h"
#include "Player.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "AudioManager.h"

class Enemy : public EntityBox2D
{
public:
	//Enum used to play the correct animation based on the current state
	enum AnimState
	{
		IDLE,
		RUN,
		JUMP,
		IN_AIR,
		ATTACK,
		JUMP_ATTACK,
		NUM_STATES
	};

	//Enum used to play the correct sound from the sounds array
	enum EnemySound
	{
		JUMP_SOUND,
		ATTACK_SOUND,
		NUM_SOUNDS
	};

	Enemy();
	~Enemy();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position,
		const glm::vec2& dimensions, const Colour& colour, const Texture textures[],
		const SoundEffect sounds[], const bool fixedRotation = true);

	void update(Player* player, std::vector<Marker*>& markerEntities, std::vector<Marker*>& collisionBoxEntities);

	//Animate, cull then add Enemy to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const int getHealth() const { return mHealth; }
	inline const bool getDead() const { return mDead; }

	inline const void setHealth(const int health) { mHealth = health; }

private:
	AnimState mAnimState = IDLE;

	//Enemy has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 8.0f;

	//IDLE, RUN, JUMP, IN_AIR, ATTACK, JUMP_ATTACK
	//Array of sprite sheets for the different states
	SpriteSheet mSpriteSheets[NUM_STATES];

	//Array of sound effects
	SoundEffect mSounds[NUM_SOUNDS];

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
	bool mSearching = true;

	float mJumpTimer = 0.0f;
	float mAttackTimer = 0.0f;

	//Timing stuff for changing direction
	const int DIRECTION_TIMER_CAP = 400;
	int mDirectionTimer;

	const float AGGRO_RANGE = 12.0f;

	int mHealth = 100;
	bool mDead = false;
	bool mIsHurt = false;
	int invinsibleTimer = 0;

	//Melee attack range
	glm::vec2 mAttackRange;
	//Box in front of the enemy representing melee attack area
	glm::vec4 mAttackBox;

	const int SWORD_DAMAGE = 10;

	float JUMP_IMPULSE = 70.0f;

	//Difficulty modifiers
	float mSpeedMods[3] = { 1.0f, 1.125f, 1.25f };
	float mDamageMods[3] = { 1.0f, 1.5f, 2.0f };
	float mAggroRangeMods[3] = { 1.0f, 1.25f, 1.5f };
};

