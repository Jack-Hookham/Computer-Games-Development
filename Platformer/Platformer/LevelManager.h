#pragma once

#include <Box2D/Box2D.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

#include "Player.h"
#include "Box.h"
#include "Ground.h"

/*
Static class for loading level files
Level files are located in res/levels/
Level file format:

Level number - used to check that the file is actually a level file

Ground start flag
Ground params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a, friction, texture
Ground end flag

Box start flag
Box params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a, density, friction, texture
Box end flag

Player start flag
Player params: x position, y position, width, height, colour.r, colour.g, colour.b, colour.a
Player textures/sprites
Player sounds
Player end flag
*/

class LevelManager
{
public:
	//Load the level
	static bool loadLevel(const std::string filePath, std::unique_ptr<b2World>& world, 
		AudioManager& audioManager, Player& player, std::vector<Ground>& groundEntities,
		std::vector<Box>& boxEntities);

private:
	static void log(const std::string text);

	//Load functions for various entities
	static void loadPlayer(std::unique_ptr<b2World>& world, Player& player, AudioManager& audioManager, const std::string line);
	static void loadGround(std::unique_ptr<b2World>& world, std::vector<Ground>& groundEntities, const std::string line);
	static void loadBox(std::unique_ptr<b2World>& world, std::vector<Box>& boxEntities, const std::string line);

	//Used to generate random data for box entities which can then be put in a level file
	static void generateBoxeData(int n);
};

