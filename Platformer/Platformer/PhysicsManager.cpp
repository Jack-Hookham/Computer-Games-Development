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

	//Projectile collision with environment
	for each (Projectile* p in player->getProjectileEntities())
	{
		for each (Ground* g in groundEntities)
		{
			if (p->getPosition().x < g->getPosition().x + g->getDimensions().x &&
				p->getPosition().x + g->getDimensions().x + p->getDimensions().x > g->getPosition().x &&
				p->getPosition().y < g->getPosition().y + g->getDimensions().y &&
				p->getPosition().y + g->getDimensions().y + p->getDimensions().y > g->getPosition().y)
			{
				p->setVelocity(glm::vec2(0.0f, 0.0f));
			}
		}

		//for each (Box* b in boxEntities)
		//{
		//	if (p->getPosition().x < b->getPosition().x + b->getDimensions().x &&
		//		p->getPosition().x + b->getDimensions().x + p->getDimensions().x > b->getPosition().x &&
		//		p->getPosition().y < b->getPosition().y + b->getDimensions().y &&
		//		p->getPosition().y + b->getDimensions().y + p->getDimensions().y > b->getPosition().y)
		//	{
		//		p->setVelocity(glm::vec2(0.0f, 0.0f));
		//	}
		//}
	}

	//Step the b2World with the timestep
	world->Step(mBox2DTimeStep, mVelocityIterations, mPositionIterations);
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
