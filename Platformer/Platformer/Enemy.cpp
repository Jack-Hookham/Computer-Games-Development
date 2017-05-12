#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, 
	const Texture textures[], const SoundEffect sounds[], int id, const bool fixedRotation)
{
	//Initialise the enemy's variables
	mID = id;

	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;

	mAnimationTimer = 0.0f;
	mDirection = 1;

	mInAir = false;
	mAttacking = false;
	mJumping = false;

	mJumpTimer = 0.0f;
	mAttackTimer = 0.0f;

	for (int i = 0; i < ENEMY_NUM_STATES; i++)
	{
		mSpriteSheets[i].init(textures[i], mSheetDimensions[i]);
	}

	for (int i = 0; i < ENEMY_NUM_SOUNDS; i++)
	{
		mSounds[i] = sounds[i];
	}

	//Box body definition
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//Box shape definition
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, (dimensions.y - dimensions.x) / 2.0f);

	//Box fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	mFixtures[0] = mBody->CreateFixture(&fixtureDef);

	//Circles on the top and bottom of the player for better movement
	//Circle shape definition
	b2CircleShape circleShape;
	circleShape.m_radius = dimensions.x / 2.0f;

	//Circle fixture definition
	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = 1.0f;
	circleDef.friction = 0.3f;

	//Top
	circleShape.m_p.Set(0.0f, (mDimensions.y - mDimensions.x) / 2.0f);
	mFixtures[1] = mBody->CreateFixture(&circleDef);

	//Bottom
	circleShape.m_p.Set(0.0f, (-mDimensions.y + mDimensions.x) / 2.0f);
	mFixtures[2] = mBody->CreateFixture(&circleDef);
}

void Enemy::update()
{
	//Check if in air
	//Loop through all contact edges
	mInAir = true;
	for (b2ContactEdge* ce = mBody->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		//if touching an entity
		if (c->IsTouching())
		{
			b2WorldManifold worldManifold;
			c->GetWorldManifold(&worldManifold);

			//loop through all manifold points
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				//if edges are below the entity 
				//add small value 0.01f to account for any error
				//mDimensions.x * 0.25f accounts for the circle fixture on the bottom of the entity body
				float entityPosition = mBody->GetPosition().y - mDimensions.y * 0.5f + mDimensions.x * 0.25f + 0.01f;
				if (worldManifold.points[i].y < entityPosition)
				{
					//Entity is on the ground
					mInAir = false;
					break;
				}
			}
		}
	}
}

void Enemy::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - mDimensions.y / 2.0f);
	glm::vec2 dimensions = mDimensions;

	//Track the current tile
	int tileIndex;
	float animationSpeed = 0.2f;

	//Store the velocity
	glm::vec2 velocity = glm::vec2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);

	if (camera.isOnCamera(position, mDimensions))
	{
		//Animation logic
		//if in air
		if (mInAir)
		{
			//if attacking
			if (mAttacking)
			{
				tileIndex = 4;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mState != ENEMY_JUMP_ATTACK)
				{
					if (mState != ENEMY_ATTACK)
					{
						//mAnimationTimer = 0.0f;
					}
					mState = ENEMY_JUMP_ATTACK;
				}
			}

			//Jumping
			else if (mJumping)
			{
				tileIndex = 5;

				//if the state just started reset the animation time
				if (mState != ENEMY_JUMP)
				{
					mState = ENEMY_JUMP;
					//mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else if (velocity.y <= 0.0f)
			{
				tileIndex = 4;
				mState = ENEMY_IN_AIR;
			}
			//if rising
			else
			{
				tileIndex = 0;
				mState = ENEMY_IN_AIR;
			}
		}
		//if on ground
		else
		{
			//if attacking on ground
			if (mAttacking)
			{
				tileIndex = 3;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mState != ENEMY_ATTACK)
				{
					if (mState != ENEMY_JUMP_ATTACK)
					{
						//mAnimationTimer = 0.0f;
					}
					mState = ENEMY_ATTACK;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mDirection > 0 || (velocity.x < 0 && mDirection < 0))))
			{
				tileIndex = 5;
				//scale up the animation speed depending on the player's speed
				animationSpeed = abs(velocity.x) * 0.025f;

				//if the state just started reset the animation time
				if (mState != ENEMY_RUN)
				{
					mState = ENEMY_RUN;
					//mAnimationTimer = 0.0f;
				}
			}
			//if stood still
			else
			{
				tileIndex = 4;

				//if state just started reset the animation time
				if (mState != ENEMY_IDLE)
				{
					mState = ENEMY_IDLE;
					//mAnimationTimer = 0.0f;
				}
			}
		}

		//only play the jump animation once
		if (mJumping)
		{
			if (mJumpTimer < mNumSprites[mState])
			{
				mJumpTimer += animationSpeed;
			}
			else
			{
				mJumping = false;
				mJumpTimer = 0.0f;
			}
		}

		//only play the attack animation once
		if (mAttacking)
		{
			if (mAttackTimer < mNumSprites[mState])
			{
				mAttackTimer += animationSpeed;
			}
			else
			{
				mAttacking = false;
				mAttackTimer = 0.0f;
			}
		}

		//Adjust position and dimensions based on the current sprite
		dimensions.x *= mStateMultipliers[mState].x;
		dimensions.y *= mStateMultipliers[mState].y;
		if (mState != ENEMY_IDLE)
		{
			if (mDirection == -1)
			{
				position.x -= dimensions.x * 0.5f;
			}
			position.y -= dimensions.y * (mStateMultipliers[mState].y - mStateMultipliers[ENEMY_IDLE].y) * 0.7f;
		}

		//Increment animation time
		mAnimationTimer += animationSpeed;

		//Apply animation
		tileIndex += (int)mAnimationTimer % mNumSprites[mState];

		glm::vec4 texCoords = mSpriteSheets[mState].getTexCoords(tileIndex);

		//if moving left
		if (mDirection == -1)
		{
			//flip x texCoords
			texCoords.x += 1.0f / mSpriteSheets[mState].getDimensions().x;
			texCoords.z *= -1;
		}

		spriteBatch.addSprite(position, mDimensions, texCoords, mSpriteSheets[mState].getTexture().id, 0.0f, mColour, mBody->GetAngle());
	}
}
