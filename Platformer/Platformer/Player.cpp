#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

//Override entity init to add capsule collision to the player
void Player::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, 
	const Texture& texture, const glm::vec4& texCoords, bool fixedRotation)
{
	//Initialise the entity's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexture = texture;
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

//Cull and add Player to SpriteBatch
void Player::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - (mDimensions.y) / 2.0f);

	if (camera.isOnCamera(position, mDimensions))
	{
		spriteBatch.addQuad(position, mDimensions, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
	}
}

void Player::input(InputManager& inputManager)
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
	if (inputManager.isKeyDown(SDLK_a))
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d))
	{
		mBody->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else
	{
		mBody->SetLinearVelocity(b2Vec2(mBody->GetLinearVelocity().x * 0.5f, mBody->GetLinearVelocity().y));
	}

	//Check if in air
	//Loop through all contact edges
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
				//if edges are below the player
				if (worldManifold.points[i].y < mBody->GetPosition().y - mDimensions.y / 2.0f + 0.01f)
				{
					below = true;
					break;
				}
			}
			if (below)
			{
				//Jump
				if (inputManager.isKeyPressed(SDLK_SPACE) && mMoveState != IN_AIR)
				{
					mBody->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);
					break;
				}
			}
		}
	}
}
