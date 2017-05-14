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

	mAttackRange = glm::vec2(1.2f + mDimensions.x * 0.5f, 1.0f + mDimensions.y * 0.5f);
	mAttackBox = glm::vec4(mPosition.x + mDimensions.x * 0.1f, mPosition.y + mDimensions.y * 0.5f - mAttackRange.y,
		mAttackRange.x * mSpriteDirection, mAttackRange.y);

	mDirectionTimer = DIRECTION_TIMER_CAP;

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

//Enemy logic
void Enemy::update(Player* player, std::vector<Marker*>& markerEntities, std::vector<Marker*>& collisionBoxEntities)
{	
	EntityBox2D::update();

	if (mHealth < 0)
	{
		mHealth = 0;
	}

	if (mHealth <= 0 && !mDead)
	{
		mDead = true;
		//mBody->GetWorld()->DestroyBody(mBody);
	}
	//else
	{
		//Cap the speed
		if (mBody->GetLinearVelocity().x > MAX_SPEED)
		{
			mBody->SetLinearVelocity(b2Vec2(MAX_SPEED, mBody->GetLinearVelocity().y));
		}
		else if (mBody->GetLinearVelocity().x < -MAX_SPEED)
		{
			mBody->SetLinearVelocity(b2Vec2(-MAX_SPEED, mBody->GetLinearVelocity().y));
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

		//Direction
		//if (mBody->GetLinearVelocity().x > 0.01f)
		//{
		//	mSpriteDirection = 1;
		//}
		//
		//if (mBody->GetLinearVelocity().x < -0.01f)
		//{
		//	mSpriteDirection = -1;
		//}

		if (mDirectionTimer <= DIRECTION_TIMER_CAP)
		{
			mDirectionTimer++;
		}

		glm::vec2 playerDistance = glm::vec2(
			mPosition.x + mDimensions.x * 0.5f - (player->getPosition().x + player->getDimensions().x * 0.5f),
			mPosition.y + mDimensions.x * 0.5f - (player->getPosition().y + player->getDimensions().y * 0.5f));

		//std::cout << glm::length(playerDistance) << std::endl;

		//Player within range?
		if (glm::length(playerDistance) < AGGRO_RANGE)
		{
			mSearching = false;
		}
		else
		{
			mSearching = true;
		}

		//Search for player
		//While searching enemy moves along x axis and changes direction when it hits a marker
		if (mSearching)
		{
			if (mDirectionTimer > 50)
			{
				for each (Marker* m in markerEntities)
				{
					//Turn around if marker hit
					//if (mBody->GetPosition().x < m->getPosition().x + m->getDimensions().x &&
					//	mBody->GetPosition().x + mDimensions.x > m->getPosition().x &&
					//	mBody->GetPosition().y < m->getPosition().y + m->getDimensions().y &&
					//	mBody->GetPosition().y + mDimensions.y > m->getPosition().y)

					if (mPosition.x < m->getPosition().x + mDimensions.x * 0.5f + m->getDimensions().x * 0.5f &&
						mPosition.x + mDimensions.x * 0.5f + m->getDimensions().x * 0.5f > m->getPosition().x &&
						mPosition.y < m->getPosition().y + mDimensions.y * 0.5f + m->getDimensions().y * 0.5f &&
						mPosition.y + mDimensions.y * 0.5f + m->getDimensions().y * 0.5f > m->getPosition().y)
					{
						mMoveDirection = -mMoveDirection;
						mSpriteDirection = -mSpriteDirection;
						mDirectionTimer = 0;
						break;
					}
				}
			}
		}
		else
		{
			if (mDirectionTimer > 10)
			{
				//face the player
				if (playerDistance.x < 0.0f)
				{
					mMoveDirection = 1;
					mSpriteDirection = 1;
					mDirectionTimer = 0;
				}
				else
				{
					mMoveDirection = -1;
					mSpriteDirection = -1;
					mDirectionTimer = 0;
				}
			}

			//if player is > 4m above
			if (player->getPosition().y > mPosition.y + 4.0f)
			{
				for each (Marker* m in markerEntities)
				{
					//Jump if inside a marker
					if (mPosition.x < m->getPosition().x + mDimensions.x * 0.5f + m->getDimensions().x * 0.5f &&
						mPosition.x + mDimensions.x * 0.5f + m->getDimensions().x * 0.5f > m->getPosition().x &&
						mPosition.y < m->getPosition().y + mDimensions.y * 0.5f + m->getDimensions().y * 0.5f &&
						mPosition.y + mDimensions.y * 0.5f + m->getDimensions().y * 0.5f > m->getPosition().y)
					{
						if (!mInAir && !mJumping)
						{
							//Jump
							mBody->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), b2Vec2(0.0f, 0.0f), true);
							mJumping = true;
							mSounds[ENEMY_JUMP_SOUND].play();
						}
					}
				}
			}
		}

		if (mIsHurt)
		{
			invinsibleTimer++;
			if (invinsibleTimer > 40)
			{
				mIsHurt = false;
				invinsibleTimer = 0;
			}
		}

		//*****Calculate whether enemy has been hit by player*****
		//Draw the attack box (debugging)
		//collisionBoxEntities[0]->setPosition(player->getAttackBox().x, player->getAttackBox().y);
		//collisionBoxEntities[0]->setDimensions(player->getAttackBox().z, player->getAttackBox().w);

		//if player is attacking and enemy isn't already hurt
		if (player->getAttacking() && !mIsHurt)
		{
			//left of player
			if (mPosition.x < player->getPosition().x)
			{
				if (/*player->getAttacking() && !mIsHurt &&*/
					mPosition.x < player->getAttackBox().x + mDimensions.x * 0.5f - player->getAttackBox().z * 0.5f &&
					mPosition.x + mDimensions.x * 0.5f - player->getAttackBox().z * 0.5f > player->getAttackBox().x &&
					mPosition.y < player->getAttackBox().y + mDimensions.y * 0.5f + player->getAttackBox().w * 0.5f &&
					mPosition.y + mDimensions.y * 0.5f + player->getAttackBox().w * 0.5f > player->getAttackBox().y)
				{
					std::cout << "hit\n";
					mIsHurt = true;
					mHealth -= 20;

					mBody->ApplyForceToCenter(b2Vec2(-20000.0f * mMoveDirection, 1000.0f), true);
				}
			}
			//right of player
			else
			{
				if (/*player->getAttacking() && !mIsHurt &&*/
					mPosition.x < player->getAttackBox().x + mDimensions.x * 0.5f + player->getAttackBox().z * 0.5f &&
					mPosition.x + mDimensions.x * 0.5f + player->getAttackBox().z * 0.5f > player->getAttackBox().x &&
					mPosition.y < player->getAttackBox().y + mDimensions.y * 0.5f + player->getAttackBox().w * 0.5f &&
					mPosition.y + mDimensions.y * 0.5f + player->getAttackBox().w * 0.5f > player->getAttackBox().y)
				{
					std::cout << "hit\n";
					mIsHurt = true;
					mHealth -= player->getSwordDamage();

					mBody->ApplyForceToCenter(b2Vec2(-20000.0f * mMoveDirection, 1000.0f), true);
				}
			}
		}

		//*****Calculate whether to attack*****
		//Update the attack box
		mAttackBox = glm::vec4(mPosition.x + mDimensions.x * 0.1f, mPosition.y + mDimensions.y * 0.5f - mAttackRange.y,
			mAttackRange.x * mSpriteDirection, mAttackRange.y);

		//Draw the attack box (debugging)
		collisionBoxEntities[0]->setPosition(mAttackBox.x, mAttackBox.y);
		collisionBoxEntities[0]->setDimensions(mAttackBox.z, mAttackBox.w);

		if (!mAttacking)
		{
			//left of player
			if (mPosition.x > player->getPosition().x)
			{
				if (/*player->getAttacking() && !mIsHurt &&*/
					player->getPosition().x < mAttackBox.x + player->getDimensions().x * 0.5f - mAttackBox.z * 0.5f &&
					player->getPosition().x + player->getDimensions().x * 0.5f - mAttackBox.z * 0.5f > mAttackBox.x &&
					player->getPosition().y < mAttackBox.y + player->getDimensions().y * 0.5f + mAttackBox.w * 0.5f &&
					player->getPosition().y + player->getDimensions().y * 0.5f + mAttackBox.w * 0.5f > mAttackBox.y)
				{
					mSounds[PLAYER_ATTACK_SOUND].play();
					mAttacking = true;
					std::cout << "enemy hit\n";
					player->setHealth(player->getHealth() - SWORD_DAMAGE);
				}
			}
			//right of player
			else
			{
				if (/*player->getAttacking() && !mIsHurt &&*/
					player->getPosition().x < mAttackBox.x + player->getDimensions().x * 0.5f + mAttackBox.z * 0.5f &&
					player->getPosition().x + player->getDimensions().x * 0.5f + mAttackBox.z * 0.5f > mAttackBox.x &&
					player->getPosition().y < mAttackBox.y + player->getDimensions().y * 0.5f + mAttackBox.w * 0.5f &&
					player->getPosition().y + player->getDimensions().y * 0.5f + mAttackBox.w * 0.5f > mAttackBox.y)
				{
					mSounds[PLAYER_ATTACK_SOUND].play();
					mAttacking = true;
					std::cout << "enemy hit\n";
					player->setHealth(player->getHealth() - SWORD_DAMAGE);
				}
			}
		}

		//Move if not hurt
		if (!mIsHurt && !mAttacking)
		{
			mBody->ApplyForceToCenter(b2Vec2(100.0f * mMoveDirection, 0.0f), true);
		}
	}
}

void Enemy::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x * 0.5f, mBody->GetPosition().y - mDimensions.y * 0.5f);
	glm::vec2 dimensions = mDimensions;

	//Track the current tile
	int tileIndex;
	float animationSpeed = 0.2f;

	//Store the velocity
	glm::vec2 velocity = glm::vec2(mBody->GetLinearVelocity().x, mBody->GetLinearVelocity().y);

	if (camera.isOnCamera(position, mDimensions))
	{
		//Animation logic
		if (mDead)
		{

		}
		//if in air
		if (mInAir)
		{
			//if attacking
			if (mAttacking)
			{
				tileIndex = 4;
				animationSpeed = 0.4f;

				//if the state just started reset the animation time
				if (mAnimState != ENEMY_JUMP_ATTACK)
				{
					if (mAnimState != ENEMY_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = ENEMY_JUMP_ATTACK;
				}
			}

			//Jumping
			else if (mJumping)
			{
				tileIndex = 5;

				//if the state just started reset the animation time
				if (mAnimState != ENEMY_JUMP)
				{
					mAnimState = ENEMY_JUMP;
					mAnimationTimer = 0.0f;
				}
			}
			//if falling
			else if (velocity.y <= 0.0f)
			{
				tileIndex = 4;
				mAnimState = ENEMY_IN_AIR;
			}
			//if rising
			else
			{
				tileIndex = 0;
				mAnimState = ENEMY_IN_AIR;
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
				if (mAnimState != ENEMY_ATTACK)
				{
					if (mAnimState != ENEMY_JUMP_ATTACK)
					{
						mAnimationTimer = 0.0f;
					}
					mAnimState = ENEMY_ATTACK;
				}
			}

			//if moving
			else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && mSpriteDirection > 0 || (velocity.x < 0 && mSpriteDirection < 0))))
			{
				tileIndex = 5;
				//scale up the animation speed depending on the player's speed
				animationSpeed = abs(velocity.x) * 0.025f;

				//if the state just started reset the animation time
				if (mAnimState != ENEMY_RUN)
				{
					mAnimState = ENEMY_RUN;
					mAnimationTimer = 0.0f;
				}
			}
			//if stood still
			else
			{
				tileIndex = 4;

				//if state just started reset the animation time
				if (mAnimState != ENEMY_IDLE)
				{
					mAnimState = ENEMY_IDLE;
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

		//Adjust position and dimensions based on the current sprite
		dimensions.x *= mStateMultipliers[mAnimState].x;
		dimensions.y *= mStateMultipliers[mAnimState].y;
		if (mAnimState != ENEMY_IDLE)
		{
			if (mSpriteDirection == -1)
			{
				position.x -= dimensions.x * 0.5f;
			}
			position.y -= dimensions.y * (mStateMultipliers[mAnimState].y - mStateMultipliers[ENEMY_IDLE].y) * 0.7f;
		}

		//Increment animation time
		mAnimationTimer += animationSpeed;

		//Apply animation
		tileIndex += (int)mAnimationTimer % mNumSprites[mAnimState];

		glm::vec4 texCoords = mSpriteSheets[mAnimState].getTexCoords(tileIndex);

		//if moving left
		if (mSpriteDirection == -1)
		{
			//flip x texCoords
			texCoords.x += 1.0f / mSpriteSheets[mAnimState].getDimensions().x;
			texCoords.z *= -1;
		}

		spriteBatch.addSprite(position, dimensions, texCoords, mSpriteSheets[mAnimState].getTexture().id, 0.0f, mColour, mBody->GetAngle());
	}
}
