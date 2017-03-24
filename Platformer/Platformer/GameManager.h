#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

//#include "PlatformerCommon.h"
#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "Player.h"
#include "Timer.h"
#include "Common.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	//Start up SDL and create window
	bool init();

	//Main loop for the game
	int gameLoop();

	//Log for GameManager
	void log(const std::string text);

private:
	//const int SCREEN_WIDTH = 1280;
	//const int SCREEN_HEIGHT = 720;

	const int SCREEN_FPS = 60;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	Player* mPlayer;
	PhysicsManager* mPhysicsManager;
	GraphicsManager* mGraphicsManager;

	//Application timer
	Timer mTimer;

	//FPS timer
	Timer mFPSTimer;

	//The frames per second cap timer
	//MOVE THESE TIMERS
	Timer mCapTimer;
};

