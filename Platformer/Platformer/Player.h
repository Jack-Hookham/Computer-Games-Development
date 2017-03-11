#pragma once
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

private:
	Vector3 mPosition;
	Camera mCamera;
};

