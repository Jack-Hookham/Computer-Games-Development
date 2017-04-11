#include "Player.h"

Player::Player()
{
	mPosition = Vector3(0, 0, 0);
}

Player::Player(const int x, const int y)
{
	mPosition = Vector3(x, y);
	mHealth = 100.0f;

	mSpeed = 3.0f;

	mVelX = 0.0f;
	mVelY = 0.0f;

	mHeight = 2.0f;
	mWidth = 2.0f;
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

float Player::getHealth()
{
	return mHealth;
}

void Player::setHealth(float health)
{
	mHealth = health;
}

float Player::getSpeed()
{
	return mSpeed;
}

void Player::setSpeed(float speed)
{
	mSpeed = speed;
}

float Player::getHeight()
{
	return mHeight;
}

void Player::setHeight(float height)
{
	mHeight = height;
}

float Player::getWidth()
{
	return mWidth;
}

void Player::setWidth(float width)
{
	mWidth = width;
}

void Player::move()
{
	mPosition += Vector3(mVelX, mVelY, 0);

	//If the player went too far to the left or right
	if ((mPosition.getX() < 0) || (mPosition.getX() + mWidth > SCREEN_WIDTH))
	{
		//Move back
		mPosition -= Vector3(mVelX, 0, 0);
	}
}

void Player::moveLeft()
{
	log("Move Left");
	mPosition += Vector3(-mSpeed, 0, 0);
	cout << mPosition << endl;
};

void Player::moveRight()
{
	log("Move Right");
	mPosition += Vector3(mSpeed, 0, 0);
	cout << mPosition << endl;
}

void Player::jump()
{
	log("Move Jump");
}

void Player::setVelX(float vel)
{
	mVelX = vel;
}

float Player::getVelX()
{
	return mVelX;
}

void Player::setVelY(float vel)
{
	mVelY = vel;
}

float Player::getVelY()
{
	return mVelY;
}

void Player::log(const std::string text)
{
	std::cout << "[Player] " << text << std::endl;
}
