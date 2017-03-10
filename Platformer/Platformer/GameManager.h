#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

#include "GameLoop.h"
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

private:
	GameLoop* mGameLoop;
	PhysicsManager* mPhysicsManager;
	GraphicsManager* mGraphicsManager;
	Player* mPlayer;
};

