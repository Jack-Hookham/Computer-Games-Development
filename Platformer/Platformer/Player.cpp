#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::input(InputManager& inputManager)
{
	if (inputManager.isKeyDown(SDLK_a))
	{
		mBody->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	if (inputManager.isKeyDown(SDLK_d))
	{
		mBody->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
}
