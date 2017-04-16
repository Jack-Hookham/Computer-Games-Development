#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

bool PhysicsManager::initPhysics()
{
	return false;
}

void PhysicsManager::updatePhysics(std::vector<Bullet> &bullets)
{
	for (int i = 0; i < bullets.size();)
	{
		if (bullets[i].update() == true) 
		{
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
		else
		{
			i++;
		}
	}
}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
