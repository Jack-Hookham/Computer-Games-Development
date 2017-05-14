#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	for each (Kunai* k in mKunaiEntities)
	{
		delete k;
	}
}

//Override entity init to add capsule collision to the player
void Player::init(b2World* world, const glm::vec2& position,
	const glm::vec2& dimensions, const Colour& colour, const Texture textures[],
	 const SoundEffect sounds[], const bool fixedRotation)
{
	//Initialise the player's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;

	mAttackRange = glm::vec2(1.2f + mDimensions.x * 0.5f, 1.0f + mDimensions.y * 0.5f);
	mAttackBox = glm::vec4(mPosition.x + mDimensions.x * 0.1f, mPosition.y + mDimensions.y * 0.5f - mAttackRange.y,
		mAttackRange.x * mDirection, mAttackRange.y);

	for (int i = 0; i < PLAYER_NUM_STATES; i++)
	{
		mSpriteSheets[i].init(textures[i], mSheetDimensions[i]);
	}

	for (int i = 0; i < PLAYER_NUM_SOUNDS; i++)
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
	boxShape.SetAsBox(dimensions.x * 0.5f, (dimensions.y - dimensions.x) * 0.5f);

	//Box fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	mFixtures[0] = mBody->CreateFixture(&fixtureDef);

	//Circles on the top and bottom of the player for better movement
	//Circle shape definition
	b2CircleShape circleShape;
	circleShape.m_radius = dimensions.x * 0.5f;

	//Circle fixture definition
	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = 1.0f;
	circleDef.friction = 0.3f;

	//Top
	circleShape.m_p.Set(0.0f, (mDimensions.y - mDimensions.x) * 0.5f);
	mFixtures[1] = mBody->CreateFixture(&circleDef);

	//Bottom
	circleShape.m_p.Set(0.0f, (-mDimensions.y + mDimensions.x) * 0.5f);
	mFixtures[2] = mBody->CreateFixture(&circleDef);
}

void Player::update()
{
	EntityBox2D::update();

	mAttackBox = glm::vec4(mPosition.x + mDimensions.x * 0.1f, mPosition.y + mDimensions.y * 0.5f - mAttackRange.y,
		mAttackRange.x * mDirection, mAttackRange.y);

	//Cap the player's speed
	if (mBody->GetLinearVelocity().x > MAX_SPEED)
	{
		mBody->SetLinearVelocity(b2Vec2(MAX_SPEED, mBody->GetLinearVelocity().y));
	}
	else if (mBody->GetLinearVelocity().x < -MAX_SPEED)
	{
		mBody->SetLinearVelocity(b2Vec2(-MAX_SPEED, mBody->GetLinearVelocity().y));
	}

	for each (Kunai* k in mKunaiEntities)
	{
		k->update();
	}


	//Delete kunai after period of time
	for (auto it = mKunaiEntities.begin(); it != mKunaiEntities.end();)
	{
		if ((*it)->getLifeTimer().getTicks() > KUNAI_LIFE_SPAN)
		{
			delete *it;
			it = mKunaiEntities.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Player::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x * 0.5f, mBody->GetPosition().y - mDimensions.y * 0.5f);
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
				if (mAnimState != PLAYER_JUMP_ATTACK)
				{
					if (mAnimState != PLAYER_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = PLAYER_JUMP_ATTACK;
				}
			}	
			
			//if throwing
			if (mThrowing)
			{
				tileIndex = 5;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mAnimState != PLAYER_JUMP_THROW)
				{
					if (mAnimState != PLAYER_THROW)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = PLAYER_JUMP_THROW;
				}
			}

			//Jumping
			else if (mJumping)
			{
				tileIndex = 5;

				//if the state just started reset the animation time
				if (mAnimState != PLAYER_JUMP)
				{
					mAnimState = PLAYER_JUMP;
					mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else if (velocity.y <= 0.0f)
			{
				tileIndex = 4;
				mAnimState = PLAYER_IN_AIR;
			}
			//if rising
			else
			{
				tileIndex = 0;
				mAnimState = PLAYER_IN_AIR;
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
				if (mAnimState != PLAYER_ATTACK)
				{
					if (mAnimState != PLAYER_JUMP_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = PLAYER_ATTACK;
				}
			}

			//if throwing
			else if (mThrowing)
			{
				tileIndex = 5;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mAnimState != PLAYER_THROW)
				{
					if (mAnimState != PLAYER_JUMP_THROW)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = PLAYER_THROW;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mDirection > 0 || (velocity.x < 0 && mDirection < 0))))
			{
				tileIndex = 5;
				//scale up the animation speed depending on the player's speed
				animationSpeed = abs(velocity.x) * 0.025f;

				//if the state just started reset the animation time
				if (mAnimState != PLAYER_RUN)
				{
					mAnimState = PLAYER_RUN;
					mAnimationTimer = 0.0f;
				}
			}
			//if stood still
			else
			{
				tileIndex = 4;

				//if state just started reset the animation time
				if (mAnimState != PLAYER_IDLE)
				{
					mAnimState = PLAYER_IDLE;
					mAnimationTimer = 0.0f;
				}
			}
		}

		//only play the jump animation once
		if (mJumping)
		{
			if (mJumpTimer < mNumSprites[mAnimState])
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
			if (mAttackTimer < mNumSprites[mAnimState])
			{
				mAttackTimer += animationSpeed;
			}
			else
			{
				mAttacking = false;
				mAttackTimer = 0.0f;
			}
		}		
		
		//only play the throw animation once
		if (mThrowing)
		{
			if (mThrowTimer < mNumSprites[mAnimState])
			{
				mThrowTimer += animationSpeed;
			}
			else
			{
				mThrowing = false;
				mThrowTimer = 0.0f;
			}
		}

		//Adjust position and dimensions based on the current sprite
		dimensions.x *= mStateMultipliers[mAnimState].x;
		dimensions.y *= mStateMultipliers[mAnimState].y;
		if (mAnimState != PLAYER_IDLE)
		{
			if (mDirection == -1)
			{
				position.x -= dimensions.x * 0.5f;
			}
			position.y -= dimensions.y * (mStateMultipliers[mAnimState].y - mStateMultipliers[PLAYER_IDLE].y) * 0.7f;
		}

		//Increment animation time
		mAnimationTimer += animationSpeed;

		//Apply animation
		tileIndex += (int)mAnimationTimer % mNumSprites[mAnimState];

		glm::vec4 texCoords = mSpriteSheets[mAnimState].getTexCoords(tileIndex);

		//if moving left
		if (mDirection == -1)
		{
			//flip texCoords
			texCoords.x += 1.0f / mSpriteSheets[mAnimState].getDimensions().x;
			texCoords.z *= -1.0f;
		}

		spriteBatch.addSprite(position, dimensions, texCoords, mSpriteSheets[mAnimState].getTexture().id, 
			0.0f, mColour, mBody->GetAngle());
	}

	for each (Kunai* k in mKunaiEntities)
	{
		k->add(spriteBatch, camera);
	}
}

void Player::input(InputManager& inputManager)
{
	//Left and right movement
	if ((inputManager.isKeyDown(SDLK_a) || inputManager.getLeftStickDirection() < 0) && mAnimState != PLAYER_ATTACK && mAnimState != PLAYER_THROW)
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		mDirection = -1;
	}
	else if ((inputManager.isKeyDown(SDLK_d) || inputManager.getLeftStickDirection() > 0) && mAnimState != PLAYER_ATTACK && mAnimState != PLAYER_THROW)
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
		//if touching an entity
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
					mBody->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), b2Vec2(0.0f, 0.0f), true);
					mJumping = true;
					mSounds[PLAYER_JUMP_SOUND].play();
					break;
				}
			}
		}
	}

	//Attack
	if (!mAttacking && (inputManager.isKeyPressed(SDLK_SPACE) || inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_X)))
	{
		mSounds[PLAYER_ATTACK_SOUND].play();
		mAttacking = true;
	}

	if (!mThrowing && (
		inputManager.isKeyPressed(SDLK_q) || inputManager.isKeyPressed(SDLK_q) || 
		inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_Y) || inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_B)))
	{
		mSounds[PLAYER_THROW_SOUND].play();
		mThrowing = true;

		if (inputManager.isKeyPressed(SDLK_q) || inputManager.isKeyPressed(SDL_CONTROLLER_BUTTON_Y))
		{
			Kunai* kunai1 = new Kunai;
			Kunai* kunai2 = new Kunai;
			Kunai* kunai3 = new Kunai;

			glm::vec2 kunaiPos = glm::vec2(mPosition.x - mDimensions.x * 0.5f, mPosition.y);
			glm::vec2 kunaiDims = glm::vec2(1.0f, 0.2f);
			Colour kunaiColour = { 255, 255, 255, 255 };
			Texture kunaiTexture = ResourceManager::getTexture(mKunaiPath);
			glm::vec4 kunaiTexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

			//if moving left
			if (mDirection == -1)
			{
				//flip texCoords
				kunaiTexCoords.x += 1.0f / kunaiDims.x;
				kunaiTexCoords.z *= -1.0f;
			}

			glm::vec2 kunaiVel1 = glm::vec2(0.05f * mDirection, 0.0f);

			kunai1->init(kunaiPos, kunaiDims, kunaiColour, kunaiTexture, kunaiVel1, kunaiTexCoords);

			mKunaiEntities.emplace_back(kunai1);
		}
	}
}
