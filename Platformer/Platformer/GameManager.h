#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "Player.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	//Start up SDL and create window
	bool init();

	//Main loop for the game
	int gameLoop();

	void log(const char* text);

private:
	PhysicsManager* mPhysicsManager;
	GraphicsManager* mGraphicsManager;
	Player* mPlayer;
};

