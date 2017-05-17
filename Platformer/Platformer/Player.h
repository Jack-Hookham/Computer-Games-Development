#pragma once

#include "EntityBox2D.h"
#include "InputManager.h"
#include "SpriteSheet.h"
#include "AudioManager.h"
#include "Projectile.h"

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

class Player : public EntityBox2D
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
		THROW,
		JUMP_THROW,
		NUM_STATES
	};

	//Enum used to play the correct sound from the sounds array
	enum PlayerSound
	{
		JUMP_SOUND,
		ATTACK_SOUND,
		THROW_SOUND,
		NUM_SOUNDS
	};

	Player();
	~Player();

	//Override entity init to add capsule collision to the player
	virtual void init(b2World* world, const glm::vec2& position, 
		const glm::vec2& dimensions, const Colour& colour, const Texture textures[], 
		const SoundEffect sounds[], const bool fixedRotation = true);

	void update();

	void input(InputManager& inputManager);

	//Animate, cull then add Player to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	const bool getAttacking() const { return mAttacking; }
	const bool getDead() const { return mDead; }
	const int getHealth() const { return mHealth; }
	const int getDirection() const { return mDirection; }
	const glm::vec4 getAttackBox() const { return mAttackBox; }
	const int getSwordDamage() const { return SWORD_DAMAGE; }
	const int getShurikenDamage() const { return SHURIKEN_DAMAGE; }
	const std::vector<Projectile*>& getProjectileEntities() const { return mProjectileEntities; }
	const int getDifficulty() const { return mDifficulty; }

	inline std::vector<Projectile*>& updateProjectileEntities() { return mProjectileEntities; }

	//non const version of getBody so that the body can be updated
	b2Body* updateBody() const { return mBody; }

	const void setHealth(const int health) { mHealth = health; }
	const void setPosition(const glm::vec2 position) { mPosition = position; }
	const void setDifficulty(const int difficulty) { mDifficulty = difficulty; }
	const void setDead(const bool dead);

private:
	void log(const std::string text);

	AnimState mAnimState = IDLE;

	//Player has 3 fixtures - middle square and a circle on the top and bottom
	//This creates a capsule shape and gives smoother movement
	b2Fixture* mFixtures[3];

	//Limit the player's speed
	const float MAX_SPEED = 10.0f;
	
	//IDLE, RUN, JUMP, IN_AIR, ATTACK, JUMP_ATTACK
	//Array of sprite sheets for the different states
	SpriteSheet mSpriteSheets[NUM_STATES];	
	
	//Array of sound effects
	SoundEffect mSounds[NUM_SOUNDS];

	//Number of sprites for each state
	int mNumSprites[NUM_STATES] = { 10, 10, 3, 1, 10, 10, 10, 10};

	//Rows and columns in the sprite sheet
	glm::ivec2 mSheetDimensions[NUM_STATES] = { glm::ivec2(4, 3), glm::ivec2(5, 2), glm::ivec2(5, 2), 
		glm::ivec2(5, 2), glm::ivec2(3, 4), glm::ivec2(4, 3), glm::ivec2(5, 2), glm::ivec2(5, 2) };

	//Multipliers to adjust the sprite position and dimension based on the state (new dimensions/base dimensions)
	glm::vec2 mStateMultipliers[NUM_STATES] = { glm::vec2(1.0f), glm::vec2(1.56f, 1.04f),  glm::vec2(1.56f, 1.1f),
		glm::vec2(1.56f, 1.1f), glm::vec2(2.31f, 1.13f), glm::vec2(2.17f, 1.18f), glm::vec2(1.63f, 1.03f), glm::vec2(1.55f, 0.98f) };

	//Animation timer
	float mAnimationTimer = 0.0f;

	int mDirection = 1;

	//State bool checks
	bool mInAir = true;
	bool mJumping = false;
	bool mAttacking = false;
	bool mThrowing = false;
	bool mDead = false;

	float mJumpTimer = 0.0f;
	float mAttackTimer = 0.0f;
	float mThrowTimer = 0.0f;

	int mHealth = 100;
	//Melee attack range
	glm::vec2 mAttackRange;
	//Box in front of the player representing melee attack area
	glm::vec4 mAttackBox;

	const int SWORD_DAMAGE = 50;
	const int SHURIKEN_DAMAGE = 10;

	std::vector<Projectile*> mProjectileEntities;
	const Texture mProjectileTexture = ResourceManager::getTexture("../res/textures/ninja_adventure/player/png/shuriken.png");

	//Projectile stuff
	const int PROJECTILE_LIFE_SPAN = 10000;

	glm::vec2 mShurikenDims = glm::vec2(0.5f, 0.5f);
	float mShurikenSpeed = 0.5f;

	//Timing stuff for staggering multiple projectile spawns in a line
	const int LINE_SPAWN_TIME = 100;
	Timer mLineSpawnTimer;
	int mLineProjectiles = 0;

	//Timing stuff for staggering multiple projectile spawns in a spread
	const int SPREAD_SPAWN_TIME = 50;
	Timer mSpreadSpawnTimer;
	int mSpreadProjectiles = 0;
	float mSpreadMultiplierY = -1.0f;

	float JUMP_IMPULSE = 70.0f;

	int mDifficulty = 0;
};

