#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
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
