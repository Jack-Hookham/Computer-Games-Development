#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	for each (Projectile* k in mProjectileEntities)
	{
		delete k;
	}
	mProjectileEntities.clear();
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
	if (mHealth > 0)
	{
		EntityBox2D::update();
	}

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

	//Update each projectile
	for each (Projectile* k in mProjectileEntities)
	{
		k->update();
	}

	//Delete projectile after period of time
	for (auto it = mProjectileEntities.begin(); it != mProjectileEntities.end();)
	{
		if ((*it)->getLifeTimer().getTicks() > PROJECTILE_LIFE_SPAN)
		{
			delete *it;
			it = mProjectileEntities.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (mLineSpawnTimer.getTicks() > LINE_SPAWN_TIME)
	{
		Projectile* projectile = new Projectile;

		glm::vec2 projectilePos = glm::vec2(
			mPosition.x - mDimensions.x * 0.5f + mDimensions.x * mDirection,
			mPosition.y - mDimensions.y * 0.2f);
		glm::vec2 projectileDims = mShurikenDims;
		Colour projectileColour = { 255, 255, 255, 255 };
		glm::vec4 projectileTexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

		//if moving left
		if (mDirection == -1)
		{
			//flip texCoords
			projectileTexCoords.x += 1.0f / projectileDims.x;
			projectileTexCoords.z *= -1.0f;
		}

		glm::vec2 projectileVel = glm::vec2(mShurikenSpeed * mDirection, 0.0f);

		projectile->init(projectilePos, projectileDims, projectileColour, mProjectileTexture, projectileVel, projectileTexCoords);
		mProjectileEntities.emplace_back(projectile);

		mLineSpawnTimer.restart();
		mLineProjectiles++;
	}
	
	if (mLineProjectiles >= 3)
	{
		mLineSpawnTimer.stop();
		mLineProjectiles = 0;
	}

	if (mSpreadSpawnTimer.getTicks() > SPREAD_SPAWN_TIME)
	{
		Projectile* projectile = new Projectile;

		glm::vec2 projectilePos = glm::vec2(
			mPosition.x - mDimensions.x * 0.5f + mDimensions.x * mDirection,
			mPosition.y - mDimensions.y * 0.2f);
		glm::vec2 projectileDims = mShurikenDims;
		Colour projectileColour = { 255, 255, 255, 255 };
		glm::vec4 projectileTexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

		//if moving left
		if (mDirection == -1)
		{
			//flip texCoords
			projectileTexCoords.x += 1.0f / projectileDims.x;
			projectileTexCoords.z *= -1.0f;
		}

		glm::vec2 projectileVel = glm::normalize(glm::vec2(mDirection, 0.5f * mSpreadMultiplierY)) * mShurikenSpeed;

		projectile->init(projectilePos, projectileDims, projectileColour, mProjectileTexture, projectileVel, projectileTexCoords);
		mProjectileEntities.emplace_back(projectile);

		mSpreadSpawnTimer.restart();
		mSpreadProjectiles++;
		mSpreadMultiplierY++;
	}

	//Stop spawning projectiles and reset variables once max reached
	if (mSpreadProjectiles >= 3)
	{
		mSpreadSpawnTimer.stop();
		mSpreadProjectiles = 0;
		mSpreadMultiplierY = -1.0f;
	}

	//Delete projectiles that need deleting
	for (auto it = mProjectileEntities.begin(); it != mProjectileEntities.end();)
	{
		if ((*it)->getDelete())
		{
			delete *it;
			it = mProjectileEntities.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (mHealth < 0)
	{
		mHealth = 0;
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
				if (mAnimState != JUMP_ATTACK)
				{
					if (mAnimState != ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = JUMP_ATTACK;
				}
			}	
			
			//if throwing
			else if (mThrowing)
			{
				tileIndex = 5;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mAnimState != JUMP_THROW)
				{
					if (mAnimState != THROW)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = JUMP_THROW;
				}
			}

			//Jumping
			else if (mJumping)
			{
				tileIndex = 5;

				//if the state just started reset the animation time
				if (mAnimState != JUMP)
				{
					mAnimState = JUMP;
					mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else if (velocity.y <= 0.0f)
			{
				tileIndex = 4;
				mAnimState = IN_AIR;
			}
			//if rising
			else
			{
				tileIndex = 0;
				mAnimState = IN_AIR;
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
				if (mAnimState != ATTACK)
				{
					if (mAnimState != JUMP_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = ATTACK;
				}
			}

			//if throwing
			else if (mThrowing)
			{
				tileIndex = 5;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mAnimState != THROW)
				{
					if (mAnimState != JUMP_THROW)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = THROW;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mDirection > 0 || (velocity.x < 0 && mDirection < 0))))
			{
				tileIndex = 5;
				//scale up the animation speed depending on the player's speed
				animationSpeed = abs(velocity.x) * 0.025f;

				//if the state just started reset the animation time
				if (mAnimState != RUN)
				{
					mAnimState = RUN;
					mAnimationTimer = 0.0f;
				}
			}
			//if stood still
			else
			{
				tileIndex = 4;

				//if state just started reset the animation time
				if (mAnimState != IDLE)
				{
					mAnimState = IDLE;
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
		if (mAnimState != IDLE)
		{
			if (mDirection == -1)
			{
				position.x -= dimensions.x * 0.5f;
			}
			position.y -= dimensions.y * (mStateMultipliers[mAnimState].y - mStateMultipliers[IDLE].y) * 0.7f;
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

	for each (Projectile* k in mProjectileEntities)
	{
		k->add(spriteBatch, camera);
	}
}

void Player::input(InputManager& inputManager)
{
	//Left and right movement
	if ((inputManager.getKeyboard()->isKeyDown(SDLK_a) || inputManager.getController()->getLeftStickDirection() < 0) &&
		mAnimState != ATTACK && mAnimState != THROW)
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		mDirection = -1;
	}
	else if ((inputManager.getKeyboard()->isKeyDown(SDLK_d) || inputManager.getController()->getLeftStickDirection() > 0) &&
		mAnimState != ATTACK && mAnimState != THROW)
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
				if (inputManager.getKeyboard()->isKeyPressed(SDLK_w) || 
					inputManager.getController()->isButtonPressed(SDL_CONTROLLER_BUTTON_A))
				{
					//Jump
					mBody->ApplyLinearImpulse(b2Vec2(0.0f, JUMP_IMPULSE), b2Vec2(0.0f, 0.0f), true);
					mJumping = true;
					mSounds[JUMP_SOUND].play();
					break;
				}
			}
		}
	}

	//Attack
	if (!mAttacking && !mThrowing && (inputManager.getKeyboard()->isKeyPressed(SDLK_SPACE) ||
		inputManager.getController()->isButtonDown(SDL_CONTROLLER_BUTTON_X)))
	{
		mSounds[ATTACK_SOUND].play();
		mAttacking = true;
	}

	if (!mThrowing && !mAttacking)
	{
		if (inputManager.getMouse()->isButtonPressed(SDL_BUTTON_LEFT) || inputManager.getController()->isButtonDown(SDL_CONTROLLER_BUTTON_B))
		{
			mSounds[THROW_SOUND].play();
			mThrowing = true;

			Projectile* projectile = new Projectile;

			glm::vec2 projectilePos = glm::vec2(
				mPosition.x - mDimensions.x * 0.5f + mDimensions.x * mDirection, 
				mPosition.y - mDimensions.y * 0.2f);
			glm::vec2 projectileDims = mShurikenDims;
			Colour projectileColour = { 255, 255, 255, 255 };
			glm::vec4 projectileTexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

			//if moving left
			if (mDirection == -1)
			{
				//flip texCoords
				projectileTexCoords.x += 1.0f / projectileDims.x;
				projectileTexCoords.z *= -1.0f;
			}

			glm::vec2 projectileVel = glm::vec2(mShurikenSpeed * mDirection, 0.0f);

			projectile->init(projectilePos, projectileDims, projectileColour, mProjectileTexture, projectileVel, projectileTexCoords);
			mProjectileEntities.emplace_back(projectile);

			mLineSpawnTimer.start();
			mLineProjectiles++;
		}

		else if (inputManager.getMouse()->isButtonPressed(SDL_BUTTON_RIGHT) || inputManager.getController()->isButtonDown(SDL_CONTROLLER_BUTTON_Y))
		{
			mSounds[THROW_SOUND].play();
			mThrowing = true;

			Projectile* projectile = new Projectile;

			glm::vec2 projectilePos = glm::vec2(
				mPosition.x - mDimensions.x * 0.5f + mDimensions.x * mDirection,
				mPosition.y - mDimensions.y * 0.2f);
			glm::vec2 projectileDims = mShurikenDims;
			Colour projectileColour = { 255, 255, 255, 255 };
			glm::vec4 projectileTexCoords = { 0.0f, 0.0f, 1.0f, 1.0f };

			//if moving left
			if (mDirection == -1)
			{
				//flip texCoords
				projectileTexCoords.x += 1.0f / projectileDims.x;
				projectileTexCoords.z *= -1.0f;
			}

			glm::vec2 projectileVel = glm::normalize(glm::vec2(mDirection, 0.5f * mSpreadMultiplierY)) * mShurikenSpeed;

			projectile->init(projectilePos, projectileDims, projectileColour, mProjectileTexture, projectileVel, projectileTexCoords);
			mProjectileEntities.emplace_back(projectile);

			mSpreadSpawnTimer.start();
			mSpreadProjectiles++;
			mSpreadMultiplierY++;
		}
	}
}

const void Player::setDead(const bool dead)
{
	mDead = dead;
}

void Player::log(const std::string text)
{
	std::cout << "[Player] " << text << std::endl;
}