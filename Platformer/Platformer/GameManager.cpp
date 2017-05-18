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

		//Load the background music files
		mMenuMusic = mAudioManager.loadMusic("../res/sound/8_bit_pack/bgm_menu.mp3");
		mGameMusic = mAudioManager.loadMusic("../res/sound/8_bit_pack/bgm_action_4.mp3");

		//Load sound effects
		mPlaceBoxSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1A.wav");
		mPlaceGroundSound = mAudioManager.loadSoundEffect("../res/sound/Menu_FX/Item1B.wav"); 
	}
	else
	{
		log("Failed to initialise audio");
		failedInits++;
	}

	//Initialise the Box2D world and entities
	initWorld();

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

	ShowCursor(FALSE);

	//Load default enemy textures
	mEnemyTextures[0] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/idle.png");
	mEnemyTextures[1] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/run.png");
	mEnemyTextures[2] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/jump.png");
	mEnemyTextures[3] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/jump.png");
	mEnemyTextures[4] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/attack.png");
	mEnemyTextures[5] = ResourceManager::getTexture("../res/textures/ninja_adventure/enemy/spritesheet/jump_attack.png");
	//Load defualt enemy sounds
	mEnemySounds[0] = mAudioManager.loadSoundEffect("../res/sound/platformer_jumping/jump_01.wav");
	mEnemySounds[1] = mAudioManager.loadSoundEffect("../res/sound/melee_sounds/sword_sound.wav");

	return failedInits;
}

void GameManager::initWorld()
{
	if (!mWorldInitialised)
	{
		//Set up the world
		mPlayer = new Player;
		mWorldManager.generateWorld(mMainLevelPath, mAudioManager, mPlayer, mGroundEntities, mBoxEntities, mEnemyEntities,
			mMarkerEntities, mEnemySpawnPositions);

		mWorldInitialised = true;
	}
}

int GameManager::gameLoop()
{
	//Play the background music file
	mMenuMusic.play(-1);

	while (mGameState != QUIT)
	{
		switch (mGameState)
		{
		case MENU:
			menuLoop();

		case PLAY:
			playLoop();

		case GAMEOVER:
			gameOverLoop();
		}
	}

	quit();

	return 0;
}

void GameManager::menuLoop()
{

	mInputManager.update();

	while (mGameState == MENU)
	{
		manageInput();

		//Update graphics
		mGraphicsManager.clearBuffers();
		mGraphicsManager.drawMenu();
		mGraphicsManager.swapBuffers();

		//Select difficulty
		if (mInputManager.getKeyboard()->isKeyPressed(SDLK_z) || 
			mInputManager.getController()->isButtonPressed(SDL_CONTROLLER_BUTTON_A))
		{
			mDifficulty = EASY;
			mGameState = PLAY;
			break;
		}		
		
		//Play game if space press
		if (mInputManager.getKeyboard()->isKeyPressed(SDLK_x) ||
			mInputManager.getController()->isButtonPressed(SDL_CONTROLLER_BUTTON_X))
		{
			mDifficulty = NORMAL;
			mGameState = PLAY;
			break;
		}
		
		//Play game if space press
		if (mInputManager.getKeyboard()->isKeyPressed(SDLK_c) ||
			mInputManager.getController()->isButtonPressed(SDL_CONTROLLER_BUTTON_B))
		{
			mDifficulty = HARD;
			mGameState = PLAY;
			break;
		}

		//Quit game if escape pressed
		if (mInputManager.getKeyboard()->isKeyPressed(SDLK_ESCAPE))
		{
			mGameState = QUIT;
			break;
		}

		mInputManager.update();
	}
}

void GameManager::gameOverLoop()
{
	mInputManager.update();

	//Play the background music file
	mMenuMusic.play(-1);

	//Move the player away from the world
	//Enemies will continue to move around the world while the game over screen shows
	mPlayer->updateBody()->SetTransform(b2Vec2(1000.0f, 1000.0f), mPlayer->getBody()->GetAngle());
	mPlayer->setDead(true);

	//Count the number of frames to calculate fps
	int frameCount = 0;

	//Calculate the score
	float aggression = -1.0f;
	//Ensure no divide by 0
	if (mRoundTime != 0)
	{
		aggression = (float)mKills / (float)mRoundTime * 5.0f;
	}
	//score = roundTime * aggression * difficulty
	//score = kills * difficulty * 5
	int score = mKills * mScoreMods[mDifficulty] * 5;

	//Highscores stuff
	Highscores highscores = Highscores();
	int rank = highscores.addScore(score);
	highscores.writeScores();

	while (mGameState == GAMEOVER)
	{
		//Start cap timer at the start of each frame (each loop)
		mFrameTimer.start();

		//Spawn enemies if below enemy limit
		if (mEnemyEntities.size() < MAX_ENEMIES)
		{
			spawnEnemy();
		}

		//Update physics
		mPhysicsManager.updatePhysics(mWorldManager.world, mPlayer, mBoxEntities, mGroundEntities, mEnemyEntities,
			mMarkerEntities, mCollisionBoxEntities, mKills);

		manageInput();

		//Update graphics
		mGraphicsManager.clearBuffers();
		mGraphicsManager.drawGame(mPlayer, mBoxEntities, mGroundEntities, mEnemyEntities);
		mGraphicsManager.drawGameOver(mRoundTime, mKills, aggression, mScoreMods[mDifficulty], score, highscores, rank);
		mGraphicsManager.swapBuffers();

		//Go to menu if buttons pressed
		if (mInputManager.getKeyboard()->isKeyPressed(SDLK_ESCAPE) ||
			mInputManager.getController()->isButtonPressed(SDL_CONTROLLER_BUTTON_START))
		{
			mGameState = MENU;
			break;
		}

		mInputManager.update();

		//If frame finished early cap the frame rate
		int frameTicks = mFrameTimer.getTicks();
		if (frameTicks < mScreenTicksPerFrame)
		{
			//Wait remaining time
			SDL_Delay(mScreenTicksPerFrame - (float)frameTicks);
		}
	}
	deleteEntities();
}

void GameManager::playLoop()
{
	mInputManager.update();

	//Play the background music file
	mGameMusic.play(-1);

	initWorld();

	//Count the number of frames to calculate fps
	int frameCount = 0;
	mFPSTimer.restart();
	mRoundTimer.restart();

	//Set variables
	float fps = 0.0f;
	mScore = 0;
	mKills = 0;
	mRoundTimer.restart();

	mPlayer->setDifficulty(mDifficulty);

	while (mGameState == PLAY)
	{
		//Start cap timer at the start of each frame (each loop)
		mFrameTimer.restart();

		//Spawn enemies if below enemy limit
		if (mEnemyEntities.size() <= MAX_ENEMIES)
		{
			spawnEnemy();
		}

		//Manage the user input
		manageInput();
		processInput();

		//Update physics
		mPhysicsManager.updatePhysics(mWorldManager.world, mPlayer, mBoxEntities, mGroundEntities, mEnemyEntities,
			mMarkerEntities, mCollisionBoxEntities, mKills);

		if (mPlayer->getHealth() <= 0)
		{
			mGameState = GAMEOVER;
		}

		//Calculate fps
		int tickCount = mFPSTimer.getTicks();
		//if over 1 second has passed
		if (tickCount > MS_PER_SECOND)
		{
			fps = frameCount * MS_PER_SECOND / tickCount;
			mFPSTimer.restart();
			frameCount = 0;
		}
		//Increment the frame counter
		frameCount++;

		mRoundTime = mRoundTimer.getTicks() / MS_PER_SECOND;

		//Update graphics
		mGraphicsManager.clearBuffers();

		mGraphicsManager.drawGame(mPlayer, mBoxEntities, mGroundEntities, mEnemyEntities);
		mGraphicsManager.drawHUD(fps, mRoundTime, mKills, mPlayer);

		mGraphicsManager.swapBuffers();

		//If frame finished early cap the frame rate
		int frameTicks = mFrameTimer.getTicks();
		if (frameTicks < mScreenTicksPerFrame)
		{
			//Wait remaining time
			SDL_Delay(mScreenTicksPerFrame - (float)frameTicks);
		}
	}
}

//Spawn an enemy at one of the spawn positions off screen
void GameManager::spawnEnemy()
{
	//Vector of off screen spawn positions
	std::vector<glm::vec2> possibleSpawnPositions;

	//Fill vector of off screen positions
	for each (glm::vec2 position in mEnemySpawnPositions)
	{
		//if the spawn position is off the camera add it to the vector
		if (!mGraphicsManager.getCamera().isOnCamera(position, mEnemyDims))
		{
			possibleSpawnPositions.emplace_back(position);
		}
	}

	//if > 0 possible positions found
	if (possibleSpawnPositions.size() > 0)
	{
		//Random number
		std::mt19937 randGenerator(std::rand());
		int indexMax = possibleSpawnPositions.size() - 1;
		std::uniform_int_distribution<int> indexGen(0, indexMax);
		int index = indexGen(randGenerator);

		Enemy* enemy = new Enemy;
		enemy->init(mWorldManager.world.get(), possibleSpawnPositions[index], mEnemyDims, mEnemyColour, mEnemyTextures, mEnemySounds);
		mEnemyEntities.emplace_back(enemy);
	}
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
			mInputManager.updateKeyboard()->pressKey(e.key.keysym.sym);
		}
		//Key release
		else if (e.type == SDL_KEYUP)
		{
			mInputManager.updateKeyboard()->releaseKey(e.key.keysym.sym);
		}
		//Mouse motion
		else if (e.type == SDL_MOUSEMOTION)
		{
			mInputManager.updateMouse()->setMouseCoords((float)e.motion.x, (float)e.motion.y);
		}
		//Mouse button down
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			mInputManager.updateMouse()->pressButton(e.button.button);
		}
		//Mouse button up
		else if (e.type == SDL_MOUSEBUTTONUP)
		{
			mInputManager.updateMouse()->releaseButton(e.button.button);
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
						mInputManager.updateController()->setLeftStickDirection(-1);
					}
					//Right of dead zone
					else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						mInputManager.updateController()->setLeftStickDirection(1);
					}
					//In dead zone
					else if (e.jaxis.value > -JOYSTICK_DEAD_ZONE && e.jaxis.value < JOYSTICK_DEAD_ZONE)
					{
						mInputManager.updateController()->setLeftStickDirection(0);
					}
				}
			}
		}

		//Controller button press
		else if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			mInputManager.updateController()->pressButton(e.cbutton.button);
		}
		//Controller button release
		else if (e.type == SDL_CONTROLLERBUTTONUP)
		{
			mInputManager.updateController()->releaseButton(e.cbutton.button);
		}
	}
}

//Input processing for the play loop
void GameManager::processInput()
{
	//Go to menu if escape pressed
	if (mInputManager.getKeyboard()->isKeyPressed(SDLK_ESCAPE))
	{
		mGameState = MENU;
	}

	//Manage input for the player
	mPlayer->input(mInputManager);

	//Update the input manager - copies current input maps to previous input maps
	mInputManager.update();
}

void GameManager::deleteEntities()
{
	if (mWorldInitialised)
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

		for each (Marker* m in mCollisionBoxEntities)
		{
			delete m;
		}
		mCollisionBoxEntities.clear();

		mEnemySpawnPositions.clear();

		delete mPlayer;

		mWorldInitialised = false;
	}
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


