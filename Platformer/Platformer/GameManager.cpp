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
	else
	{
		log("Physics successfully initialised");
	}

	//initialise graphics
	if (!(mGraphicsManager->initGraphics()))
	{
		log("Failed to initialise graphics");
		success = false;
	}
	else
	{
		log("Graphics successfully initialised");
	}

	return success;
}

int GameManager::gameLoop()
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set text colour as black
	SDL_Color textColour = { 0, 0, 0, 255 };

	//Current time start time
	Uint32 startTime = 0;

	while (!quit)
	{
		SDL_GetTicks();

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

				case SDLK_RETURN:
					startTime = SDL_GetTicks();
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
		mGraphicsManager->updateGraphics(startTime);
	}

	return 0;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}



