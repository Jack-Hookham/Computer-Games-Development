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
		mPlaceBoxSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1A.wav");
		mPlaceGroundSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1B.wav");
	}
	else
	{
		log("Failed to initialise audio");
		failedInits++;
	}

	//Initialise physics
	if (mPhysicsManager.initPhysics(mDesiredFPS, mWorld, mAudioManager, mPlayer,  mBoxEntities, mGroundEntities))
	{
		log("Physics successfully initialised");
	}
	else
	{
		log("Failed to initialise physics");
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

		//Manage the user input, check the players
		manageInput();

		//Update all physics
		mPhysicsManager.updatePhysics(mWorld);

		//Calculate fps
		float avgFPS = countedFrames / (mFPSTimer.getTicks() / MS_PER_SECOND);

		mGraphicsManager.updateGraphics(avgFPS, mPlayer, mBoxEntities, mGroundEntities);
		++countedFrames;

		//If frame finished early cap the frame rate
		int frameTicks = mFrameTimer.getTicks();
		if (frameTicks < mScreenTicksPerFrame)
		{
			//Wait remaining time
			SDL_Delay(mScreenTicksPerFrame - (float)frameTicks);
		}
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
	if (mInputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		//Create a box at the mouse position when LMB is pressed
		glm::vec2 mouseCoords = mInputManager.getMouseCoords();
		glm::vec2 worldCoords = mGraphicsManager.getCamera().screenToWorld(mouseCoords);
		glm::vec2 dimensions = glm::vec2(2.0f);
		Colour colour(255, 255, 255, 255);
		Texture boxTexture = ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box2.png");

		mPhysicsManager.addBoxToWorld(mBoxEntities, mWorld, worldCoords, dimensions, colour, boxTexture);

		mPlaceBoxSound.play();
	}

	if (mInputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		//Create ground at the mouse position when RMB is pressed
		glm::vec2 mouseCoords = mInputManager.getMouseCoords();
		glm::vec2 worldCoords = mGraphicsManager.getCamera().screenToWorld(mouseCoords);
		glm::vec2 dimensions = glm::vec2(1.0f);
		Colour colour(255, 255, 255, 255);
		Texture groundTexture = ResourceManager::getTexture("../res/textures/platformerArt/png/ground.png");

		mPhysicsManager.addGroundToWorld(mGroundEntities, mWorld, worldCoords, dimensions, colour, groundTexture);

		mPlaceGroundSound.play();
	}

	//Manage input for the player, check the player state and update the state if needed
	mPlayer.update(mInputManager);

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

	//Update the input manager - copys current input map to previous input map
	mInputManager.update();
}



