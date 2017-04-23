#pragma once

#include <Box2D/Box2D.h>

#include "Player.h"
#include "Box.h"
#include "Ground.h"

/*
Static class for loading level files
Level files are located in res/levels/
Level file format:

Number of ground objects
Ground params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a, friction, texture

Number of box objects
Ground params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a, density, friction, texture

Player params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a
Player textures/sprites
Player sounds
*/

class LevelManager
{
public:
	static bool loadLevel(const std::string& filePath, std::unique_ptr<b2World>& world, 
		AudioManager& audioManager, Player& player, std::vector<Ground>& groundEntities,
		std::vector<Box>& boxEntities);

private:
	static void log(const std::string text);
};

