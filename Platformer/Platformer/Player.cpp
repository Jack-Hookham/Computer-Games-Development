#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

//Override entity init to add capsule collision to the player
void Player::init(b2World* world, const glm::vec2& position,
	const glm::vec2& dimensions, const Colour& colour, const Texture textures[],
	const glm::vec4& texCoords, const SoundEffect sounds[], const bool fixedRotation)
{
	//Initialise the player's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexCoords = texCoords;

	for (int i = 0; i < NUM_STATES; i++)
	{
		mSpriteSheets[i].init(textures[i], mSheetDimensions[i]);
	}

	for (int i = 0; i < NUM_SOUNDS; i++)
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

void Player::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - mDimensions.y / 2.0f);
	glm::vec2 dimensions = mDimensions;

	//Track the current tile
	int tileIndex;
	float animationSpeed = 0.2f;

	//Store the velocity
	glm::vec2 velocity = glm::vec2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);

	//if the player is on the screen (should be always true with the current camera setup)
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
				if (mState != JUMP_ATTACK)
				{
					if (mState != ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mState = JUMP_ATTACK;
				}
			}

			//Jumping
			else if (mJumping)
			{
				tileIndex = 5;

				//if the state just started reset the animation time
				if (mState != JUMP)
				{
					mState = JUMP;
					mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else if (velocity.y <= 0.0f)
			{
				tileIndex = 4;
				mState = IN_AIR;
			}
			//if rising
			else
			{
				tileIndex = 0;
				mState = IN_AIR;
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
				if (mState != ATTACK)
				{
					if (mState != JUMP_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mState = ATTACK;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mDirection > 0 || (velocity.x < 0 && mDirection < 0))))
			{
				tileIndex = 5;
				//scale up the animation speed depending on the player's speed
				animationSpeed = abs(velocity.x) * 0.025f;

				//if the state just started reset the animation time
				if (mState != RUN)
				{
					mState = RUN;
					mAnimationTimer = 0.0f;
				}
			}
			//if stood still
			else
			{
				tileIndex = 4;

				//if state just started reset the animation time
				if (mState != IDLE)
				{
					mState = IDLE;
					mAnimationTimer = 0.0f;
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
		if (mState != IDLE)
		{
			if (mDirection == -1)
			{
				position.x -= dimensions.x * 0.5f;
			}
			position.y -= dimensions.y * (mStateMultipliers[mState].y - mStateMultipliers[IDLE].y) * 0.7f;
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

		spriteBatch.addSprite(position, dimensions, texCoords, mSpriteSheets[mState].getTexture().id, 
			0.0f, mColour, mBody->GetAngle());
	}
}

void Player::update(InputManager& inputManager)
{
	//Cap the player's speed
	if (mBody->GetLinearVelocity().x > MAX_SPEED)
	{
		mBody->SetLinearVelocity(b2Vec2(MAX_SPEED, mBody->GetLinearVelocity().y));
	}
	else if (mBody->GetLinearVelocity().x < -MAX_SPEED)
	{
		mBody->SetLinearVelocity(b2Vec2(-MAX_SPEED, mBody->GetLinearVelocity().y));
	}

	//Left and right movement
	if (inputManager.isKeyDown(SDLK_a) && mState != ATTACK)
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		mDirection = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d) && mState != ATTACK)
	{
		mDirection = 1;
		mBody->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else
	{
		mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x * 0.5f, mBody->GetLinearVelocity().y));
	}

	//Check if in air
	//Loop through all contact edges
	mInAir = true;
	for (b2ContactEdge* ce = mBody->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;
		//if touching and entity
		if (c->IsTouching())
		{
			b2WorldManifold worldManifold;
			c->GetWorldManifold(&worldManifold);

			//loop through all manifold points
			for (int i = 0; i < b2_maxManifoldPoints; i++)
			{
				//if edges are below the player 
				//add small value 0.01f to account for any error
				//mDimensions.x * 0.25f accounts for the circle fixture on the bottom of the player body
				if (worldManifold.points[i].y < mBody->GetPosition().y - mDimensions.y * 0.5f + mDimensions.x * 0.25f + 0.01f)
				{
					//Player is on the ground
					mInAir = false;
					break;
				}
			}
			if (!mInAir)
			{
				//Can jump
				if (inputManager.isKeyPressed(SDLK_w) || inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_A))
				{
					//Jump
					mBody->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					mJumping = true;
					mSounds[JUMP_SOUND].play();
					break;
				}
			}
		}
	}

	//Attack
	if (inputManager.isKeyPressed(SDLK_SPACE) || inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_X))
	{
		mSounds[ATTACK_SOUND].play();
		mAttacking = true;
	}
}
