#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

int GameManager::run()
{
	int failedInits = init();
	if (failedInits > 0)
	{
		const std::string text = "Failed to initialise " + std::to_string(failedInits) + "";
		log("Failed to initialise " + std::to_string(failedInits) + " subsystems");
	}
	else
	{
		log("All subsystems successfully initialised");
	}

	return gameLoop();
}

int GameManager::init()
{
	//Track the number of failed inits
	int failedInits = 0;

	//Initialise graphics
	//Done before the PhysicsManager because the PhysicsManager adds textures to entities
	//using the ResourceManager which requires OpenGl to be setup first
	if (mGraphicsManager.initGraphics(mScreenWidth, mScreenHeight))
	{
		log("Graphics successfully initialised");
	}
	else
	{
		log("Failed to initialise graphics");
		failedInits++;
	}

	//Initialise physics
	if (mPhysicsManager.initPhysics(mDesiredFPS, mB2World, mEntities))
	{
		log("Physics successfully initialised");
	}
	else
	{
		log("Failed to initialise physics");
		failedInits++;
	}

	//Initialise audio
	//Audio must initialise after graphics because SDL is initialised in graphics
	//Audio uses SDL_mixer
	if (mAudioManager.initAudio())
	{
		log("Audio successfully initialised");

		//Set initial volume (max 128)
		mAudioManager.setVolume(30);

		//Load the background music file
		mMusic = mAudioManager.loadMusic("../res/sound/8_bit_pack/bgm_action_3.mp3");
		//Play the background music file
		mMusic.play(-1);

		//Load sound effect
		mShotSound1 = mAudioManager.loadSoundEffect("../res/sound/tutorial/shots/cg1.wav");
	}
	else
	{
		log("Failed to initialise audio");
		failedInits++;
	}

	return failedInits;
}

int GameManager::gameLoop()
{
	//Number of frames
	int countedFrames = 0;

	mFPSTimer.start();

	Uint32 previousTicks = SDL_GetTicks();

	while (mGameState != QUIT)
	{
		//Start cap timer at the start of each frame (each loop)
		mFrameTimer.start();

		//Manage the user input
		manageInput();

		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalTimeStep = frameTime / mScreenTicksPerFrame;
		
		//Physics update
		int stepCount = 0;

		//This loop ensures that physics updates are not effected by fps
		while (totalTimeStep > 0.0f && stepCount < MAX_PHYSICS_STEPS)
		{
			//Set timestep to the smaller of the two
			//This means that if the totalTimeStep is bigger than the max time step allowed
			//We limit the timestep to the max time step
			float timeStep = std::min(totalTimeStep, MAX_TIME_STEP);
			
			//Update all physics
			mPhysicsManager.updatePhysics(mB2World, mEntities);

			totalTimeStep -= timeStep;
			stepCount++;
			//std::cout << "Step Count " << stepCount << std::endl;
		}

		//Calculate and correct fps
		float avgFPS = countedFrames / (mFPSTimer.getTicks() / MS_PER_SECOND);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		mGraphicsManager.updateGraphics(avgFPS, mEntities);
		++countedFrames;

		//cout << "FPS: " << avgFPS << endl;

		//If frame finished early
		int frameTicks = mFrameTimer.getTicks();
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
		glm::vec2 worldCoords = mGraphicsManager.getCamera().screenToWorld(mouseCoords);
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
		mGraphicsManager.translateCamera(glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (mInputManager.isKeyDown(SDLK_d))
	{
		mGraphicsManager.translateCamera(glm::vec2(CAMERA_SPEED, 0.0f));
	}

	if (mInputManager.isKeyDown(SDLK_w))
	{
		mGraphicsManager.translateCamera(glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (mInputManager.isKeyDown(SDLK_s))
	{
		mGraphicsManager.translateCamera(glm::vec2(0.0f, -CAMERA_SPEED));
	}

	if (mInputManager.isKeyDown(SDLK_q))
	{
		//zoom in
		mGraphicsManager.setCameraScale(SCALE_SPEED);
	}
	if (mInputManager.isKeyDown(SDLK_e))
	{
		//zoom out
		mGraphicsManager.setCameraScale(-SCALE_SPEED);
	}

	//Timers
	//if (mInputManager.isKeyPressed(SDLK_s))
	//{
	//	if (mTimer.isStarted())
	//	{
	//		mTimer.stop();
	//	}
	//	else
	//	{
	//		mTimer.start();
	//	}
	//}
	//if (mInputManager.isKeyPressed(SDLK_p))
	//{
	//	if (mTimer.isPaused())
	//	{
	//		mTimer.unpause();
	//	}
	//	else
	//	{
	//		mTimer.pause();
	//	}
	//}
	//if (mInputManager.isKeyPressed(SDLK_r))
	//{
	//	mTimer.restart();
	//}

	//Update the input manager - copys current input map to previous input map
	mInputManager.update();
}



