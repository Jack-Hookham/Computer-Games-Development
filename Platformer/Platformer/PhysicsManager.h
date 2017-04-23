#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>

#include "Entity.h"
#include "Player.h"
#include "Box.h"
#include "Ground.h"
#include "LevelManager.h"

//Manages the physics updates as well as the box2D world
//The majority of the physics stuff is done by box2D
//The main function of the PhysicsManager is just to call the box2D step() function

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics(const int desiredFPS);

	//Update the world
	void updatePhysics(std::unique_ptr<b2World>& world);

private:
	//Log for PhysicsManager
	void log(const std::string text);

	//The timestep used for the box2D step function
	float mBox2DTimeStep;

	//Velocity phase and position phase values for the box2d step function
	//These are the values suggested in the box2d manual
	int32 mVelocityIterations = 8;
	int32 mPositionIterations = 3;
};

