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

void PhysicsManager::updatePhysics()
{

}

void PhysicsManager::log(const std::string text)
{
	std::cout << "[PhysicsManager] " << text << std::endl;
}
