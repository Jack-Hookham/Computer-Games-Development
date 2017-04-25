#pragma once

#include <iostream>

#include "Player.h"
#include "Box.h"
#include "Ground.h"
#include "LevelManager.h"

//Manages the box2D world

class WorldManager
{
public:
	WorldManager();
	~WorldManager();

	//Generate the box2D world
	std::unique_ptr<b2World> generateWorld(const std::string levelPath, AudioManager& audioManager, Player& player,
		std::vector<Ground>& groundEntities, std::vector<Box>& boxEntities);

	//Add a ground entity to the world after it has been created
	void addGroundToWorld(std::vector<Ground>& groundEntities, std::unique_ptr<b2World>& world, const glm::vec2& position,
		const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float friction = 0.3f, 
		const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, const bool fixedRotation = true);

	//Add a box entity to the world after it has been created
	void addBoxToWorld(std::vector<Box>& boxEntities, std::unique_ptr<b2World>& world, const glm::vec2& position,
		const glm::vec2& dimensions, const Colour& colour, const Texture& texture, const float density = 1.0f,
		const float friction = 0.3f, const glm::vec4& texCoords = { 0.0f, 0.0f, 1.0f, 1.0f }, const bool fixedRotation = false);

private:
	void log(const std::string text);
};
