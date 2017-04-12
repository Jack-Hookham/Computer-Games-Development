#pragma once
#include <iostream>
#include <string>

#include "Player.h"

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics();

	void updatePhysics();

private:
	//Log for PhysicsManager
	void log(const std::string text);
};

