#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "Player.h"
#include "Timer.h"
#include "InputManager.h"
#include "Bullet.h"

enum GameState
{
	MENU,
	PLAY,
	QUIT
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	//Run the game
	int run();

private:
	//Start up SDL and create window
	bool init();

	//Main loop for the game
	int gameLoop();	
	
	//Log for GameManager
	void log(const std::string text);

	void manageInput();

	GameState mGameState;

	int mScreenWidth;
	int mScreenHeight;

	//Desired fps for the game
	const int DESIRED_FPS = 60;

	//Store this as const as it's used multiple times
	const float MS_PER_SECOND = 1000.0f;

	//Store this to save multiple divides
	const float SCREEN_TICKS_PER_FRAME = MS_PER_SECOND / DESIRED_FPS;

	//Physics step limiter
	const int MAX_PHYSICS_STEPS = 6;
	const float MAX_TIME_STEP = 1.0f;

	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.01f;

	Player* mPlayer;
	PhysicsManager* mPhysicsManager;
	GraphicsManager* mGraphicsManager;
	InputManager mInputManager;

	//Application timer
	Timer mTimer;

	//FPS timer
	Timer mFPSTimer;

	//Timer for each frame
	Timer mFrameTimer;

	float mTimeMod;

	std::vector<Bullet> mBullets;
};

