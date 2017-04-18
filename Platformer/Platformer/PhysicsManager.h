#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Box.h"

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics(int desiredFPS, std::unique_ptr<b2World>& b2World, std::vector<Entity>& entities, Player& player);

	//Update the world
	void updatePhysics(std::unique_ptr<b2World>& world, std::vector<Entity>& entities, Player& player);

	void addBoxToWorld(std::unique_ptr<b2World>& b2World, std::vector<Entity>& entities);

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

