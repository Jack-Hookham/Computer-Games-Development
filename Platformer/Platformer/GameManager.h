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
#include "Highscores.h"

//Manager for the whole engine


class GameManager
{
public:
	enum GameState
	{
		MENU,
		PLAY,
		GAMEOVER,
		QUIT
	};

	enum Difficulty
	{
		EASY,
		NORMAL,
		HARD,
		NUM_DIFFICULTIES
	};

	GameManager();
	~GameManager();

	//Run the game
	int run();

private:
	//Start up SDL and create window
	int init();

	//Initialise the game world
	void initWorld();

	//Main loop for the game
	int gameLoop();	

	//Menu loop
	void menuLoop();

	//Show score, highscores etc
	void gameOverLoop();

	//Loop for playing the game
	void playLoop();

	void spawnEnemy();

	//Delete entities
	void deleteEntities();

	//Delete entity pointers, close SDL controller stuff
	void quit();
	
	//Log for GameManager
	void log(const std::string text);

	//Manage user input
	void manageInput();

	//Process input when playing the game
	void processInput();

	//Current game state
	GameState mGameState = MENU;

	int mScreenWidth = 1600;
	int mScreenHeight= 900;

	//Desired fps for the game
	int mDesiredFPS = 60;

	//Store this as const as it's used multiple times
	const float MS_PER_SECOND = 1000.0f;

	//Store this to save multiple divides
	float mScreenTicksPerFrame = MS_PER_SECOND / mDesiredFPS;

	const float CAMERA_SPEED = 1.0f;
	const float SCALE_SPEED = 0.1f;

	PhysicsManager mPhysicsManager;
	GraphicsManager mGraphicsManager;
	InputManager mInputManager;
	WorldManager mWorldManager;

	//Audio stuff
	AudioManager mAudioManager;
	Music mGameMusic;
	Music mMenuMusic;
	SoundEffect mPlaceBoxSound;
	SoundEffect mPlaceGroundSound;

	const std::string mMainLevelPath = "../res/levels/main_level.txt";

	//FPS timer
	Timer mFPSTimer;

	//Timer for each frame
	Timer mFrameTimer;

	//Time each round
	Timer mRoundTimer;
	int mRoundTime = 0;

	//Scoring stuff
	int mScore = 0;
	int mKills = 0;

	int mScoreMods[NUM_DIFFICULTIES] = { 1, 2, 3 };
	Difficulty mDifficulty = EASY;

	//Entities
	Player* mPlayer;
	std::vector<Box*> mBoxEntities;
	std::vector<Ground*> mGroundEntities;
	std::vector<Enemy*> mEnemyEntities;
	std::vector<Marker*> mMarkerEntities;
	std::vector<Marker*> mCollisionBoxEntities;

	std::vector<glm::vec2> mEnemySpawnPositions;

	const int MAX_ENEMIES = 50;

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;

	//Game Controller handler
	SDL_GameController* mGameController = NULL;

	//Values for spawned enemies not loaded from file
	glm::vec2 mEnemyDims = glm::vec2(1.0f, 2.0f);
	Colour mEnemyColour = { 255, 255, 255, 255 };
	Texture mEnemyTextures[Enemy::NUM_STATES];
	SoundEffect mEnemySounds[Enemy::NUM_SOUNDS];

	bool mWorldInitialised = false;
};

