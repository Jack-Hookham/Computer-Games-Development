#include "GameManager.h"

GameManager::GameManager()
{
	mPhysicsManager = new PhysicsManager();
	mGraphicsManager = new GraphicsManager();
	mPlayer = new Player();
}

GameManager::~GameManager()
{
	delete mPhysicsManager;
	delete mGraphicsManager;
	delete mPlayer;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();

}

bool GameManager::init()
{
	//initialisation flag
	bool success = true;

	//initialise physics
	if (!(mPhysicsManager->initPhysics()))
	{
		log("Failed to initialise physics");
		success = false;
	}

	//initialise graphics
	if (!(mGraphicsManager->initGraphics()))
	{
		log("Failed to initialise graphics");
		success = false;
	}

	return success;
}

int GameManager::gameLoop()
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event event;

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&event) != 0)
		{
			//User requests quit
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			mPhysicsManager->updatePhysics();

			//mGraphicsManager->loadTexture("PATH");
			mGraphicsManager->updateGraphics();
		}
	}

	return 0;
}

void GameManager::log(const char* text)
{
	std::cout << "[GameManager] " << text << std::endl;
}



