#include "Player.h"

Player::Player()
{
	mPosition = Vector3(0, 0, 0);
}

Player::Player(const int x, const int y)
{
	mPosition = Vector3(x, y);
}

Player::~Player()
{
}

Vector3 Player::getPosition()
{
	return mPosition;
}

void Player::setPosition(const int x, const int y)
{
	mPosition = Vector3(x, y);
}

void Player::setPosition(const int x, const int y, const int z)
{
	mPosition = Vector3(x, y, z);
}

void Player::setPosition(const Vector3 position)
{
	mPosition = position;
}
