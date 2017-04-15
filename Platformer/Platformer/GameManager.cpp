#include "GameManager.h"

GameManager::GameManager() : mScreenWidth(1280), mScreenHeight(720), mGameState(GameState::PLAY), mTimeMod(0)
{
	mPlayer = new Player(mScreenWidth, mScreenHeight);

	mPhysicsManager = new PhysicsManager();
	mGraphicsManager = new GraphicsManager(mPlayer, mScreenWidth, mScreenHeight);
}

GameManager::~GameManager()
{
	delete mPhysicsManager;
	delete mGraphicsManager;
	delete mPlayer;
}

int GameManager::run()
{
	if (init())
	{
		log("All systems successfully initialised");
	}
	else
	{
		log("Failed to initialise 1 or more sub systems");
	}

	return gameLoop();
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

	//Set text colour as black
	SDL_Color textColour = { 0, 0, 0, 255 };

	int countedFrames = 0;

	mFPSTimer.start();

	while (!quit)
	{
		//Start cap timer at the start of each frame (each loop)
		mCapTimer.start();

		mTimeMod += 0.01;

		quit = manageInput();

		mPlayer->move();

		//Calculate and correct fps
		float avgFPS = countedFrames / (mFPSTimer.getTicks() / 1000.0f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		mPhysicsManager->updatePhysics();

		//mGraphicsManager->loadTexture("PATH");
		mGraphicsManager->updateGraphics(mTimer, avgFPS, mTimeMod);
		++countedFrames;

		//If frame finished early
		int frameTicks = mCapTimer.getTicks();
		if (frameTicks < mScreenTicksPerFrame)
		//mMaxFPS = 60;
		//float mScreenTicksPerFrame
		{
			//Wait remaining time
			SDL_Delay(mScreenTicksPerFrame - frameTicks);
		}
	}

	return 0;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}

bool GameManager::manageInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			return true;
		}
		//Key press
		else if (e.type == SDL_KEYDOWN)
		{
			mInputManager.pressKey(e.key.keysym.sym);
		}
		//Key release
		else if (e.type == SDL_KEYUP)
		{
			mInputManager.releaseKey(e.key.keysym.sym);
		}
		//Mouse motion
		else if (e.type == SDL_MOUSEMOTION)
		{
			mInputManager.setMouseCoords((float)e.motion.x, (float)e.motion.y);
		}
		//Mouse button down
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			mInputManager.pressKey(e.button.button);
		}
		//Mouse button up
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			mInputManager.releaseKey(e.button.button);
		}
	}

	//Process input
	//Quit if escape pressed
	if (mInputManager.isKeyPressed(SDLK_ESCAPE))
	{
		return true;
	}

	//Timers
	if (mInputManager.isKeyPressed(SDLK_s))
	{
		if (mTimer.isStarted())
		{
			mTimer.stop();
		}
		else
		{
			mTimer.start();
		}
	}
	if (mInputManager.isKeyPressed(SDLK_p))
	{
		if (mTimer.isPaused())
		{
			mTimer.unpause();
		}
		else
		{
			mTimer.pause();
		}
	}
	if (mInputManager.isKeyPressed(SDLK_r))
	{
		mTimer.restart();
	}

	//Movement
	if (mInputManager.isKeyDown(SDLK_a))
	{
		//mPlayer->setVelX(-(mPlayer->getSpeed()));
		mGraphicsManager->translateCamera(glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (mInputManager.isKeyDown(SDLK_d))
	{
		//mPlayer->setVelX(mPlayer->getSpeed());
		mGraphicsManager->translateCamera(glm::vec2(CAMERA_SPEED, 0.0f));
	}

	if (mInputManager.isKeyDown(SDLK_w))
	{
		mGraphicsManager->translateCamera(glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (mInputManager.isKeyDown(SDLK_s))
	{
		mGraphicsManager->translateCamera(glm::vec2(0.0f, -CAMERA_SPEED));
	}

	//if (!mInputManager.isKeyDown(SDLK_a) && !mInputManager.isKeyDown(SDLK_d))
	//{
	//	mPlayer->setVelX(0);
	//}

	if (mInputManager.isKeyDown(SDLK_q))
	{
		//zoom in
		mGraphicsManager->setCameraScale(SCALE_SPEED);
	}
	if (mInputManager.isKeyDown(SDLK_e))
	{
		//zoom out
		mGraphicsManager->setCameraScale(-SCALE_SPEED);
	}


	//case SDLK_SPACE:
	//	mPlayer->jump();
	//	break;

	//Update the input manager - copys current input map to previous input map
	mInputManager.update();

	return false;
}



