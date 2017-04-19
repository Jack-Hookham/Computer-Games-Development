#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>

#include "Entity.h"
#include "Player.h"
#include "Box.h"
#include "Ground.h"

//Manages the physics updates as well as the box2D world

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	//initialise box2D physics
	bool initPhysics(const int desiredFPS, std::unique_ptr<b2World>& world, Player& player,
		std::vector<Box>& boxEntities, std::vector<Ground>& groundEntities);

	//Update the world
	void updatePhysics(std::unique_ptr<b2World>& world);

	void generateGround(std::unique_ptr<b2World>& world, std::vector<Ground>& groundEntities);

	void generateBoxes(std::unique_ptr<b2World>& world, std::vector<Box>& entities);

	//Add a ground entity to the world
	void addGroundToWorld(std::vector<Ground>& entities, std::unique_ptr<b2World>& world, const glm::vec2& position,
		const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float density = 0.0f,
		const float friction = 0.3f, const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, const bool fixedRotation = true);

	//Add a box entity to the world
	void addBoxToWorld(std::vector<Box>& entities, std::unique_ptr<b2World>& world, const glm::vec2& position, 
		const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float density = 1.0f, 
		const float friction = 0.3f, const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, const bool fixedRotation = false);

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

