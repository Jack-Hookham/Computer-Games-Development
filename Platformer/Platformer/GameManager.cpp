#include "GameManager.h"

GameManager::GameManager()
{
	mPlayer = new Player(SCREEN_WIDTH/2, SCREEN_WIDTH/2);
	mPhysicsManager = new PhysicsManager();
	mGraphicsManager = new GraphicsManager(mPlayer, SCREEN_WIDTH, SCREEN_HEIGHT);
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
	SDL_Event e;

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					mPlayer->moveLeft();
					break;

				case SDLK_d:
					mPlayer->moveRight();
					break;

				case SDLK_SPACE:
					mPlayer->jump();
					break;

				/*default:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;*/
				}
			}
		}



		mPhysicsManager->updatePhysics();

		//mGraphicsManager->loadTexture("PATH");
		mGraphicsManager->updateGraphics();
	}

	return 0;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}



