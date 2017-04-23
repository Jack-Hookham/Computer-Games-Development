#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::initPhysics(const int desiredFPS)
{
	log("Initialising");

	//Initialisation flag
	bool success = true;

	//Timestep variable for the box2D step function
	mBox2DTimeStep = 1.0f / desiredFPS;

	if (success)
	{
		log("Initialised");
	}

	return success;
}

//Update the world
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world)
{
	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
