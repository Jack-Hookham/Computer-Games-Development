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
				break;
			}

			//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				//Select action based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_a:
					mPlayer->movingLeft = true;
					break;

				case SDLK_d:
					mPlayer->movingRight = true;
					break;

				case SDLK_SPACE:
					mPlayer->jump();
					break;

				/*default:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;*/
				}
			}
			else if (e.type == SDL_KEYUP)
			{
				//Select action based on key release
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					mPlayer->movingLeft = false;
					break;

				case SDLK_d:
					mPlayer->movingRight = false;
					break;/*

				case SDLK_SPACE:
					mPlayer->jump();
					break;*/

					/*default:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;*/
				}
			}

			if (mPlayer->movingLeft)
			{
				mPlayer->moveLeft();
			}
			else if (mPlayer->movingRight)
			{
				mPlayer->moveRight();
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



