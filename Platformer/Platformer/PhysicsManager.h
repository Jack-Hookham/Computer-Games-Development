#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "Entity.h"
#include "Player.h"
#include "Box.h"

//Manages the physics updates as well as the box2D world

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics(int desiredFPS, std::unique_ptr<b2World>& b2World, std::vector<Entity>& entities, Player& player);

	//Update the world
	void updatePhysics(std::unique_ptr<b2World>& world, std::vector<Entity>& entities, Player& player);

	void addBoxToWorld(std::vector<Entity>& entities, std::unique_ptr<b2World>& world,
		const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture,
		const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f });

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

