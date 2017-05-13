#include "GameManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

//Run the game - initalises subsystems then runs the game loop
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

//Initialise subsystems
int GameManager::init()
{
	log("Initialising subsystems");
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

		//Load sound effect
		mPlaceBoxSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1A.wav");
		mPlaceGroundSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1B.wav");
	}
	else
	{
		log("Failed to initialise audio");
		failedInits++;
	}

	//Initialise the box2D world
	mPlayer = new Player;
	mWorld = mWorldManager.generateWorld(mMainLevelPath, mAudioManager, mPlayer, mGroundEntities, mBoxEntities, mEnemyEntities,
		mMarkerEntities);

	//Initialise physics
	if (mPhysicsManager.initPhysics(mDesiredFPS))
	{
		log("Physics successfully initialised");
	}
	else
	{
		log("Failed to initialise physics");
		failedInits++;
	}

	//Check for joysticks
	log("Searching for joysticks");
	if (SDL_NumJoysticks() < 1)
	{
		log("No joysticks connected!\n");
	}
	//if there is a joystick connected
	else
	{
		//Open the first available controller
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i))
			{
				log("Joystick " + std::to_string(i) + " supports the game controller interface");
				mGameController = SDL_GameControllerOpen(i);

				if (mGameController != NULL)
				{
					log("Controller " + std::to_string(i) + " successfully initialised");
					break;
				}
				else
				{
					log("Could not open gamecontroller " + std::to_string(i) + ": " + std::string(SDL_GetError()));
				}
			}
		}
	}

	return failedInits;
}

int GameManager::gameLoop()
{
	//Play the background music file
	mMusic.play(-1);

	//Count the number of frames to calculate fps
	int frameCount = 0;
	mFPSTimer.start();

	float fps = 0.0f;

	while (mGameState != QUIT)
	{
		//Start cap timer at the start of each frame (each loop)
		mFrameTimer.start();

		//Manage the user input, check the players
		manageInput();

		//Update all physics
		mPhysicsManager.updatePhysics(mWorld, mPlayer, mEnemyEntities, mMarkerEntities);

		//Calculate fps
		int tickCount = mFPSTimer.getTicks();
		//if over 1 second has passed
		if (tickCount > MS_PER_SECOND)
		{
			fps = frameCount * MS_PER_SECOND / tickCount;
			mFPSTimer.start();
			frameCount = 0;
		}
		//Increment the frame counter
		frameCount++;

		//Update all graphics
		mGraphicsManager.updateGraphics(fps, mPlayer, mBoxEntities, mGroundEntities, mEnemyEntities, mMarkerEntities);

		//If frame finished early cap the frame rate
		int frameTicks = mFrameTimer.getTicks();
		if (frameTicks < mScreenTicksPerFrame)
		{
			//Wait remaining time
			SDL_Delay(mScreenTicksPerFrame - (float)frameTicks);
		}
	}

	quit();

	return 0;
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

		//Controller left right movement
		else if (e.type == SDL_JOYAXISMOTION)
		{
			//Motion on controller 0
			if (e.jaxis.which == 0)
			{
				//X axis motion
				if (e.jaxis.axis == 0)
				{
					//Left of dead zone
					if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						mInputManager.setLeftStickDirection(-1);
					}
					//Right of dead zone
					else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						mInputManager.setLeftStickDirection(1);
					}
					//In dead zone
					else if (e.jaxis.value > -JOYSTICK_DEAD_ZONE && e.jaxis.value < JOYSTICK_DEAD_ZONE)
					{
						mInputManager.setLeftStickDirection(0);
					}
				}
			}
		}

		//Controller button press
		else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			mInputManager.pressKey(e.cbutton.button);
		}
		//Controller button release
		else if (e.type == SDL_CONTROLLERBUTTONUP)
		{
			mInputManager.releaseKey(e.cbutton.button);
		}	
	}

	//***Process input***
	//Quit if escape pressed
	if (mInputManager.isKeyPressed(SDLK_ESCAPE))
	{
		mGameState = QUIT;
	}

	//Reload level if r pressed
	if (mInputManager.isKeyPressed(SDLK_r))
	{
		deleteEntities();

		mPlayer = new Player;
		mWorld = mWorldManager.generateWorld(mMainLevelPath, mAudioManager, mPlayer, mGroundEntities, mBoxEntities, mEnemyEntities,
			mMarkerEntities);
	}

	//Mouse buttons
	if (mInputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		//Create a box at the mouse position when LMB is pressed
		glm::vec2 mouseCoords = mInputManager.getMouseCoords();
		glm::vec2 worldCoords = mGraphicsManager.getCamera().screenToWorld(mouseCoords);
		glm::vec2 dimensions = glm::vec2(2.0f);

		//Random colours and texture
		std::mt19937 randGenerator(time(0));

		std::uniform_int_distribution<int> colourGen(200, 255);
		std::uniform_int_distribution<int> textureGen(1, 10);

		glm::vec4 colour(colourGen(randGenerator), colourGen(randGenerator), colourGen(randGenerator), 255);
		std::string textureString = "../res/textures/boxes_and_crates/obj_box" + std::to_string(textureGen(randGenerator)) + ".png";
		Texture texture = ResourceManager::getTexture(textureString);

		mWorldManager.addBoxToWorld(mBoxEntities, mWorld, worldCoords, dimensions, colour, texture);

		mPlaceBoxSound.play();
	}

	if (mInputManager.isKeyPressed(SDL_BUTTON_RIGHT))
	{
		//Create ground at the mouse position when RMB is pressed
		glm::vec2 mouseCoords = mInputManager.getMouseCoords();
		glm::vec2 worldCoords = mGraphicsManager.getCamera().screenToWorld(mouseCoords);
		glm::vec2 dimensions = glm::vec2(1.0f);
		Colour colour(255, 255, 255, 255);
		Texture texture = ResourceManager::getTexture("../res/textures/platformerArt/png/ground.png");

		mWorldManager.addGroundToWorld(mGroundEntities, mWorld, worldCoords, dimensions, colour, texture);

		mPlaceGroundSound.play();
	}

	//Manage input for the player
	mPlayer->input(mInputManager);

	if (mInputManager.isKeyDown(SDLK_q) || mInputManager.isKeyDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
	{
		//Zoom in
		mGraphicsManager.setCameraScale(SCALE_SPEED);
	}
	if (mInputManager.isKeyDown(SDLK_e) || mInputManager.isKeyDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
	{
		//Zoom out
		mGraphicsManager.setCameraScale(-SCALE_SPEED);
	}	

	//Update the input manager - copies current input map to previous input map
	mInputManager.update();
}

void GameManager::deleteEntities()
{
	for each (Box* b in mBoxEntities)
	{
		delete b;
	}
	mBoxEntities.clear();

	for each (Ground* g in mGroundEntities)
	{
		delete g;
	}
	mGroundEntities.clear();

	for each (Enemy* e in mEnemyEntities)
	{
		delete e;
	}
	mEnemyEntities.clear();

	for each (Marker* m in mMarkerEntities)
	{
		delete m;
	}
	mMarkerEntities.clear();

	delete mPlayer;
}

//Delete entity pointers, close SDL controller stuff
void GameManager::quit()
{
	deleteEntities();

	//Close game controller
	SDL_GameControllerClose(mGameController);
	mGameController = NULL;
}

void GameManager::log(const std::string text)
{
	std::cout << "[GameManager] " << text << std::endl;
}


