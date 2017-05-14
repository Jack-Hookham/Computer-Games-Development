#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <random>

#include "PhysicsManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "WorldManager.h"
#include "AudioManager.h"
#include "Timer.h"

//Manager for the whole engine
//Left clicking on the screen will create a box at the mouse position
//Right clicking on the screen will create ground at the mouse position
//Q and E control zoom

enum GameState
{
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
	int init();

	//Main loop for the game
	int gameLoop();	

	//Delete entities
	void deleteEntities();

	//Delete entity pointers, close SDL controller stuff
	void quit();
	
	//Log for GameManager
	void log(const std::string text);

	//Manage user input
	void manageInput();

	//Current game state
	GameState mGameState = PLAY;

	int mScreenWidth = 1280;
	int mScreenHeight= 720;

	//Desired fps for the game
	int mDesiredFPS = 60;

	//Store this as const as it's used multiple times
	const float MS_PER_SECOND = 1000.0f;

	//Store this to save multiple divides
	float mScreenTicksPerFrame = MS_PER_SECOND / mDesiredFPS;

	//Physics step limiter
	const int MAX_PHYSICS_STEPS = 6;
	const float MAX_TIME_STEP = 1.0f;

	const float CAMERA_SPEED = 1.0f;
	const float SCALE_SPEED = 0.1f;

	PhysicsManager mPhysicsManager;
	GraphicsManager mGraphicsManager;
	InputManager mInputManager;
	WorldManager mWorldManager;

	const std::string mTestLevelPath = "../res/levels/test_level.txt";
	const std::string mMainLevelPath = "../res/levels/main_level.txt";

	//Audio stuff
	AudioManager mAudioManager;
	Music mMusic;
	SoundEffect mPlaceBoxSound;
	SoundEffect mPlaceGroundSound;

	//FPS timer
	Timer mFPSTimer;

	//Timer for each frame
	Timer mFrameTimer;

	//The box2D world
	std::unique_ptr<b2World> mWorld;

	//Entities
	Player* mPlayer;
	std::vector<Box*> mBoxEntities;
	std::vector<Ground*> mGroundEntities;
	std::vector<Enemy*> mEnemyEntities;
	std::vector<Marker*> mMarkerEntities;
	std::vector<Marker*> mCollisionBoxEntities;

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;

	//Game Controller handler
	SDL_GameController* mGameController = NULL;
};

