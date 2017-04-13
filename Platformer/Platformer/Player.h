#pragma once
#include <iostream>
#include <string>

#include "Vector2.h"
#include "Camera.h"

class Player
{
public:
	Player();
	Player(const int x, const int y);
	~Player();

	Vector2 getPosition();
	void setPosition(const int x, const int y);
	void setPosition(const int x, const int y, const int z);
	void setPosition(const Vector2 position);

	//Health
	float getHealth();
	void setHealth(float health);

	//Speed
	float getSpeed();
	void setSpeed(float speed);

	//Height and Width
	float getHeight();
	void setHeight(float height);
	float getWidth();
	void setWidth(float width);

	//Movement
	void move();
	void moveLeft();
	void moveRight();
	void jump();

	void setVelX(float vel);
	float getVelX();

	void setVelY(float vel);
	float getVelY();

	bool movingLeft = false;
	bool movingRight = false;
	bool jumping = false;

	void draw();

private:
	//Log for Player
	void log(const std::string text);

	Vector2 mPosition;
	Camera mCamera;

	float mHealth;
	float mSpeed;

	float mVelX;
	float mVelY;

	float mHeight;
	float mWidth;

	float mScreenWidth;
	float mScreenHeight;
};

