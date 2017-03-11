#pragma once
#include <iostream>
#include <string>

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics();

	void updatePhysics();

	//Log physics errors
	void log(const std::string text);
};

