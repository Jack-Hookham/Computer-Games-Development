#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>

#include "Player.h"
#include "Bullet.h"

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics();

	void updatePhysics(float timeStep, std::vector<Bullet> &bullets);

private:
	//Log for PhysicsManager
	void log(const std::string text);
};

