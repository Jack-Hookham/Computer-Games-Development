#include "Player.h"

Player::Player()
{
	mPosition = glm::vec2(0, 0);
}

Player::Player(int screenWidth, int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mPosition = glm::vec2(mScreenWidth / 2, mScreenHeight / 2);
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

glm::vec2 Player::getPosition()
{
	return mPosition;
}

void Player::setPosition(const int x, const int y)
{
	mPosition = glm::vec2(x, y);
}

void Player::setPosition(const int x, const int y, const int z)
{
	mPosition = glm::vec2(x, y);
}

void Player::setPosition(const glm::vec2 position)
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
	mPosition += glm::vec2(mVelX, mVelY);

	//If the player went too far to the left or right
	if ((mPosition.x < 0) || (mPosition.x + mWidth > mScreenWidth))
	{
		//Move back
		mPosition -= glm::vec2(mVelX, 0);
	}
}

void Player::moveLeft()
{
	log("Move Left");
	mPosition += glm::vec2(-mSpeed, 0);
	std::cout << mPosition.x << " " << mPosition.y << std::endl;
};

void Player::moveRight()
{
	log("Move Right");
	mPosition += glm::vec2(mSpeed, 0);
	std::cout << mPosition.x << " " << mPosition.y << std::endl;
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

void Player::draw()
{

}
