#include "GameManager.h"

GameManager::GameManager()
{
	mPhysicsManager = new PhysicsManager();
	mGraphicsManager = new GraphicsManager();
	mGameLoop = new GameLoop();

	mPlayer = new Player();
}

GameManager::~GameManager()
{
	delete mPhysicsManager;
	delete mGraphicsManager;
	delete mGameLoop;
	delete mPlayer;
}

bool GameManager::init()
{
	//Initialization flag
	bool success = true;

	//initialise physics
	if (!(mPhysicsManager->initPhysics()))
	{
		std::cout << "Failed to initialise physics";
		success = false;
	}

	//initialise graphics
	if (!(mGraphicsManager->initGraphics()))
	{
		std::cout << "Failed to initialise graphics";
		success = false;
	}

	return success;
}



