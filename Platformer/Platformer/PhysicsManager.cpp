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
void PhysicsManager::updatePhysics(std::unique_ptr<b2World>& world, Player* player, std::vector<Box*>& boxEntities, 
	std::vector<Ground*>& groundEntities, std::vector<Enemy*>& enemyEntities, std::vector<Marker*>& markerEntities,
	std::vector<Marker*>& collisionBoxEntities)
{
	player->update();

	//Enemy logic update
	for each (Enemy* e in enemyEntities)
	{
		e->update(player, markerEntities, collisionBoxEntities);
	}

	//Delete enemy if dead flag is set
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

	//Projectile collisions
	for each (Projectile* p in player->getProjectileEntities())
	{
		for each (Ground* g in groundEntities)
		{
			//check whether the squares are colliding
			if (p->getPosition().x < g->getPosition().x + g->getDimensions().x &&
				p->getPosition().x + g->getDimensions().x > g->getPosition().x &&
				p->getPosition().y < g->getPosition().y + g->getDimensions().y / 2 + p->getDimensions().y / 2 &&
				p->getPosition().y + g->getDimensions().y / 2 + p->getDimensions().y / 2 > g->getPosition().y)
			{
				p->setVelocity(glm::vec2(0.0f, 0.0f));
			}
		}
	}

	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
