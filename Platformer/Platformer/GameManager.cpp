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

	int countedFrames = 0;

	mFPSTimer.start();

	while (!quit)
	{
		//Start cap timer at the start of each frame (each loop)
		mCapTimer.start();

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

				case SDLK_s:
					if (mTimer.isStarted())
					{
						mTimer.stop();
					}
					else
					{
						mTimer.start();
					}
					break;

				case SDLK_p:
					if (mTimer.isPaused())
					{
						mTimer.unpause();
					}
					else
					{
						mTimer.pause();
					}
					break;

				case SDLK_r:
					mTimer.restart();
					break;

				case SDLK_a: mPlayer->setVelX(-(mPlayer->getSpeed())); break;

				case SDLK_d: mPlayer->setVelX(mPlayer->getSpeed()); break;

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
		}

		mPlayer->move();

		//Calculate and correct fps
		float avgFPS = countedFrames / (mFPSTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		mPhysicsManager->updatePhysics();

		//mGraphicsManager->loadTexture("PATH");
		mGraphicsManager->updateGraphics(mTimer, avgFPS);
		++countedFrames;

		//If frame finished early
		int frameTicks = mCapTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}

	return 0;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}



