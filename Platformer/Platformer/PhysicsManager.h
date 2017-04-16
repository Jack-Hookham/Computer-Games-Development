#pragma once
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

	void updatePhysics(std::vector<Bullet> &bullets);

private:
	//Log for PhysicsManager
	void log(const std::string text);
};

