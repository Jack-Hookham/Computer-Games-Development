#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

//Override entity init to add capsule collision to the player
void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, 
	const Texture textures[], const glm::vec4& texCoords, bool fixedRotation)
{
	//Initialise the entity's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mSpriteSheets[IDLE].init(textures[IDLE], glm::ivec2(4, 3));
	mSpriteSheets[RUN].init(textures[RUN], glm::ivec2(5, 2));
	mSpriteSheets[JUMP].init(textures[JUMP], glm::ivec2(5, 2));
	mSpriteSheets[ATTACK].init(textures[ATTACK], glm::ivec2(3, 4));
	mSpriteSheets[JUMP_ATTACK].init(textures[JUMP_ATTACK], glm::ivec2(4, 3));
	mTexCoords = texCoords;

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
	circleShape.m_p.Set(0.0f, (mDimensions.y - dimensions.x) / 2.0f);
	mFixtures[1] = mBody->CreateFixture(&circleDef);

	//Bottom
	circleShape.m_p.Set(0.0f, (-mDimensions.y + dimensions.x) / 2.0f);
	mFixtures[2] = mBody->CreateFixture(&circleDef);
}

void Player::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - mDimensions.y / 2.0f);
	glm::vec2 dimensions = mDimensions;

	int numTiles;
	int tileIndex;
	float animationSpeed = 0.2f;

	//Store the velocity
	glm::vec2 velocity = glm::vec2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);

	//if the player is on the screen - should be always true with the current camera setup
	if (camera.isOnCamera(position, mDimensions))
	{
		//Animation logic
		//if in air
		if (mInAir)
		{
			//Jumping
			if (mJumping)
			{
				numTiles = 10;
				tileIndex = 5;

				//Adjust position and dimensions
				dimensions.x *= 1.56f;
				dimensions.y *= 1.1f;
				if (mDirection == -1)
				{
					position.x -= dimensions.x * 0.5f;
				}

				//if the state just started reset the animation time
				if (mState != JUMP)
				{
					mState = JUMP;
					mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else
			{
				numTiles = 1;
				tileIndex = 4;
				mState = IN_AIR;

				//Adjust position and dimensions
				dimensions.x *= 1.56f;
				dimensions.y *= 1.1f;
				if (mDirection == -1)
				{
					position.x -= dimensions.x * 0.5f;
				}
			}
		}
		//if on ground
		else
		{
			//if attacking on ground
			if (mAttacking)
			{
				numTiles = 10;
				tileIndex = 3;
				animationSpeed = 0.4f;

				//Adjust position and dimensions
				dimensions.x *= 2.31;
				dimensions.y *= 1.13f;
				if (mDirection == -1)
				{
					position.x -= dimensions.x * 0.5f;
				}
				position.y -= dimensions.y * 0.1f;

				//if the state just started reset the animation time
				if (mState != ATTACK && mState != JUMP_ATTACK)
				{
					mState = ATTACK;
					mAnimationTimer = 0.0f;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mDirection > 0 || (velocity.x < 0 && mDirection < 0))))
			{
				numTiles = 10;
				tileIndex = 5;
				animationSpeed = abs(velocity.x) * 0.025f;
				
				//Adjust position and dimensions
				dimensions.x *= 1.56f;
				dimensions.y *= 1.04f;
				if (mDirection == -1)
				{
					position.x -= dimensions.x * 0.5f;
				}

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
				numTiles = 10;
				tileIndex = 4;

				//if state just started reset the animation time
				if (mState != IDLE)
				{
					mState = IDLE;
					mAnimationTimer = 0.0f;
				}
			}
		}

		if (mJumping)
		{
			if (mJumpTimer < numTiles)
			{
				mJumpTimer += animationSpeed;
			}
			else
			{
				mJumping = false;
				mJumpTimer = 0.0f;
			}
		}

		if (mAttacking)
		{
			if (mAttackTimer < numTiles)
			{
				mAttackTimer += animationSpeed;
			}
			else
			{
				mAttacking = false;
				mAttackTimer = 0.0f;
			}
		}

		//if (mAttacking)
		//{
		//	if (mJumpTimer < numTiles)
		//	{
		//		mJumpTimer += animationSpeed;
		//	}
		//	else
		//	{
		//		mJumping = false;
		//		mState = IN_AIR;
		//		mJumpTimer = 0.0f;
		//	}
		//}

		//Increment animation time
		mAnimationTimer += animationSpeed;

		//Apply animation
		tileIndex += (int)mAnimationTimer % numTiles;

		glm::vec4 texCoords = mSpriteSheets[mState].getTexCoords(tileIndex);

		//if moving left
		if (mDirection == -1)
		{
			//flip x texCoords
			texCoords.x += 1.0f / mSpriteSheets[mState].dimensions.x;
			texCoords.z *= -1;
		}

		//std::cout << "index: " << tileIndex << std::endl;
		spriteBatch.addQuad(position, dimensions, texCoords, mSpriteSheets[mState].texture.id, 
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
	if (inputManager.isKeyDown(SDLK_a) && !mAttacking)
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		mDirection = -1;
	}
	else if (inputManager.isKeyDown(SDLK_d) && !mAttacking)
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

			bool below = false;

			//loop through all manifold points
			for (unsigned int i = 0; i < b2_maxManifoldPoints; i++)
			{
				//if edges are below the player (add small value 0.01f to account for any error)
				if (worldManifold.points[i].y <  0.01f + mBody->GetPosition().y - mDimensions.y / 2.0f)
				{
					//Player is on the ground
					below = true;
					break;
				}
			}
			if (below)
			{
				mInAir = false;
				//Can jump
				if (inputManager.isKeyPressed(SDLK_w))
				{
					//Jump
					mBody->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					mJumping = true;
					break;
				}
			}
		}
	}

	if (inputManager.isKeyPressed(SDLK_SPACE))
	{
		mAttacking = true;
	}
}
