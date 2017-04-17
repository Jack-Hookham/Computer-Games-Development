#include "GameManager.h"

GameManager::GameManager()
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

	mMusic = mAudioManager.loadMusic("../res/sound/tutorial/XYZ.ogg");
	mMusic.play(-1);

	mShotSound1 = mAudioManager.loadSoundEffect("../res/sound/tutorial/shots/cg1.wav");

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

	//Audio must initialise after graphics because SDL is initialised in graphics
	//Audio uses SDL_mixer
	mAudioManager.init();

	//Box2D world setup
	b2Vec2 gravity(0.0f, -9.8f);
	mB2World = std::make_unique<b2World>(gravity);

	//Make the ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -20.0f);
	b2Body* groundBody = mB2World->CreateBody(&groundBodyDef);

	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	Box newBox;
	newBox.init(mB2World.get(), glm::vec2(0.0f, 14.0f), glm::vec2(2.0f, 2.0f));
	mBoxes.push_back(newBox);

	return success;
}

int GameManager::gameLoop()
{
	//Set text colour as black
	SDL_Color textColour = { 0, 0, 0, 255 };

	int countedFrames = 0;

	mFPSTimer.start();

	Uint32 previousTicks = SDL_GetTicks();

	while (mGameState != QUIT)
	{
		//Start cap timer at the start of each frame (each loop)
		mFrameTimer.start();
		
		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalTimeStep = frameTime / SCREEN_TICKS_PER_FRAME;

		mTimeMod += 0.01f;

		manageInput();

		//mPlayer->move();
		
		int stepCount = 0;
		//This loop ensures that physics updates are not effected by fps
		while (totalTimeStep > 0.0f && stepCount < MAX_PHYSICS_STEPS)
		{
			//Set timestep to the smaller of the two
			//This means that if the totalTimeStep is bigger than the max time step allowed
			//We limit the timestep to the max time step
			float timeStep = std::min(totalTimeStep, MAX_TIME_STEP);
			
			//Update all physics
			mPhysicsManager->updatePhysics(timeStep, mBullets);
			mB2World->Step(timeStep, 6, 2);

			totalTimeStep -= timeStep;
			stepCount++;
		}

		//Calculate and correct fps
		float avgFPS = countedFrames / (mFPSTimer.getTicks() / MS_PER_SECOND);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		//mGraphicsManager->loadTexture("PATH");
		mGraphicsManager->updateGraphics(mTimer, avgFPS, mTimeMod, mBullets, mBoxes);
		++countedFrames;

		//cout << "FPS: " << avgFPS << endl;

		//If frame finished early
		int frameTicks = mFrameTimer.getTicks();

		//if (frameTicks < SCREEN_TICKS_PER_FRAME)
		////mMaxFPS = 60;
		//{
		//	//Wait remaining time
		//	SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		//}
	}

	return 0;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}

void GameManager::manageInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			mGameState = QUIT;
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

	//***Process input***
	//Quit if escape pressed
	if (mInputManager.isKeyPressed(SDLK_ESCAPE))
	{
		mGameState = QUIT;
	}

	//Mouse buttons
	if (mInputManager.isKeyPressed(SDL_BUTTON_LEFT) || mInputManager.isKeyPressed(SDL_JOYBUTTONDOWN))
	{
		glm::vec2 mouseCoords = mInputManager.getMouseCoords();
		glm::vec2 worldCoords = mGraphicsManager->getCamera().screenToWorld(mouseCoords);
		std::cout << worldCoords.x << " " << worldCoords.y << std::endl;

		glm::vec2 playerPosition(0.0f, 0.0f);
		glm::vec2 direction = worldCoords - playerPosition;
		direction = glm::normalize(direction);

		mBullets.emplace_back(playerPosition, direction, 5.0f, 1000);

		mShotSound1.play();
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


	//case SDLK_SPACE:
	//	mPlayer->jump();
	//	break;

	//Update the input manager - copys current input map to previous input map
	mInputManager.update();
}



