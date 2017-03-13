#pragma once
#include <iostream>
#include <string>

#include "Vector3.h"
#include "Camera.h"

class Player
{
public:
	Player();
	Player(const int x, const int y);
	~Player();

	Vector3 getPosition();
	void setPosition(const int x, const int y);
	void setPosition(const int x, const int y, const int z);
	void setPosition(const Vector3 position);

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
	void moveLeft();
	void moveRight();
	void jump();

	bool movingLeft = false;
	bool movingRight = false;
	bool jumping = false;

	//Log for Player
	void log(const std::string text);

private:
	Vector3 mPosition;
	Camera mCamera;

	float mHealth = 100.0f;
	float mSpeed = 1.0f;

	float mHeight = 2.0f;
	float mWidth = 2.0f;


};

