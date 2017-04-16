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

enum class GameState
{
	MENU,
	PLAY,
	EXIT
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

	GameState mGameState;

	int mScreenWidth;
	int mScreenHeight;

	int mMaxFPS = 60;
	float mScreenTicksPerFrame = 1000 / mMaxFPS;

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

	//The frames per second cap timer
	Timer mCapTimer;

	bool manageInput();

	float mTimeMod;

	std::vector<Bullet> mBullets;
};

