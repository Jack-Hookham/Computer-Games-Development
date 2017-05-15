#pragma once

#include "EntityBox2D.h"

//Box class currently just inherits from Entity
//Used to create boxes in the world

class Box : public EntityBox2D
{
public:
	Box();
	~Box();

	void update();

private:
	float mAngle;
};

