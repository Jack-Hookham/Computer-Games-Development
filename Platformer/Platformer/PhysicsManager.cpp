#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::initPhysics(const int desiredFPS)
{
	log("Initialising physics");

	//Initialisation flag
	bool success = true;

	//Timestep variable for the box2D step function
	mBox2DTimeStep = 1.0f / desiredFPS;

	if (success)
	{
		log("Physics successfully initialised");
	}
	else
	{
		log("Failed to initialise physics");
	}

	return success;
}

//Update the world
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world, Player* player, std::vector<Enemy*>& enemyEntities,
	std::vector<Marker*>& markerEntities, std::vector<Marker*>& collisionBoxEntities)
{
	player->update();

	//Enemy logic update
	for each (Enemy* e in enemyEntities)
	{
		e->update(player, markerEntities, collisionBoxEntities);
	}

	for (auto it = enemyEntities.begin(); it != enemyEntities.end();)
	{
		if ((*it)->getDead())
		{
			delete *it;
			it = enemyEntities.erase(it);
		}
		else
		{
			it++;
		}
	}

	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
