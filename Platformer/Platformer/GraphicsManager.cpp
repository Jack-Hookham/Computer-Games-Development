#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() 
{
}

GraphicsManager::~GraphicsManager()
{
	delete mHUDFont;
	delete mMenuFont;
	delete mSmallFont;

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

bool GraphicsManager::initGraphics(const int screenWidth, const int screenHeight)
{
	log("Initialising graphics");

	//Initialisation flag
	bool success = true;

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	//Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		const std::string text = "SDL could not Initialise! SDL Error: " + std::string(SDL_GetError());
		log(text);
		success = false;
	}

	//Create the window and error check
	if (!mWindow.createWindow("Genji Simulator 2D", mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))
	{
		log("Failed to create window");
		success = false;
	}

	//Initialise the cameras
	mWorldCamera.initCamera(mScreenWidth, mScreenHeight);
	mWorldCamera.setScale(DEFAULT_CAMERA_SCALE);
	mHUDCamera.initCamera(mScreenWidth, mScreenHeight);

	//Offset the hud camera to align 0, 0 with the bottom left corner
	mHUDCamera.setPosition(glm::vec2(mScreenWidth / 2, mScreenHeight / 2));

	//Initialise SDL_ttf
	if (TTF_Init() == -1)
	{
		const std::string text = "SDL_ttf could not initialise! SDL_ttf Error: " + std::string(TTF_GetError());
		log(text);
		success = false;
	}

	//Initialise shaders
	initShaders();

	//Generate VBO and VAO to initialise the sprite batch
	mEntitySpriteBatch.bufferData();
	mHUDSpriteBatch.bufferData();
	mGameOverSpriteBatch.bufferData();
	mGameOverTextBatch.bufferData();

	//Load Menu Textures
	mMenuTexture = ResourceManager::getTexture("../res/textures/menu/MenuImage.png");
	mGameOverTexture = ResourceManager::getTexture("../res/textures/menu/GameOverImage.png");
	mEscTexture = ResourceManager::getTexture("../res/textures/menu/Esc.png");
	mStartTexture = ResourceManager::getTexture("../res/textures/menu/Start.png");

	//Load HUD textures
	mHealthTexture = ResourceManager::getTexture("../res/textures/HUD/heart.png");
	mTimeTexture = ResourceManager::getTexture("../res/textures/HUD/clock.png");
	mKillsTexture = ResourceManager::getTexture("../res/textures/HUD/dead_ninja.png");

	//Load fonts
	mHUDFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 48);
	mMenuFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 36);
	mSmallFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 24);

	if (success)
	{
		log("Graphics successfully initialised");
	}
	else
	{
		log("Failed to initialise graphics");
	}

	return success;
}

void GraphicsManager::clearBuffers()
{
	mScreenWidth = SDL_GetWindowSurface(mWindow.getSDLWindow())->w;
	mScreenHeight = SDL_GetWindowSurface(mWindow.getSDLWindow())->h;

	//Set depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader attributes
	mTextureShader.enableAttributes();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mTextureShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);
}

void GraphicsManager::swapBuffers()
{
	//Undbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader attributes
	mTextureShader.disableAttributes();

	//Swap buffer and draw everything
	mWindow.swapWindow();
}

void GraphicsManager::log(const std::string text)
{
	std::cout << "[GraphicsManager] " << text << std::endl;
}

//Compile and link the shaders used for the engine
void GraphicsManager::initShaders()
{
	log("Initialising shaders");

	//Initialise the shaders
	mTextureShader.initShaders("../res/shaders/colourVert.glsl", "../res/shaders/colourFrag.glsl");

	//Add the shader attributes
	mTextureShader.addAttribute("position");
	mTextureShader.addAttribute("colour");
	mTextureShader.addAttribute("texCoord");

	//Link the shaders
	mTextureShader.linkShaders();

	log("Shaders successfully initialised");
}

void GraphicsManager::drawMenu()
{
	//Reuse HUD camera for menu
	mHUDCamera.updateCamera();

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mGameOverSpriteBatch.begin();

	mGameOverSpriteBatch.addSprite(glm::vec2(0.0f, 0.0f), glm::vec2(mScreenWidth, mScreenHeight), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), mMenuTexture.id,
		0.0f, Colour(255, 255, 255, 255));

	mGameOverSpriteBatch.end();
	mGameOverSpriteBatch.renderBatches();
}

//Draw the game over screen
void GraphicsManager::drawGameOver(const int roundTime, const int kills, const float aggression,
	const int difficulty, const int score, Highscores& highscores, const int rank)
{
	//Reuse HUD camera for game over screen positioning
	mHUDCamera.updateCamera();

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//Render game over textures
	mGameOverSpriteBatch.begin();

	mGameOverSpriteBatch.addSprite(glm::vec2(0.0f, 0.0f), glm::vec2(mScreenWidth, mScreenHeight), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mGameOverTexture.id,
		0.0f, Colour(255, 255, 255, 255));

	mGameOverSpriteBatch.addSprite(glm::vec2(mScreenWidth * 0.32f, mScreenHeight * 0.2f), glm::vec2(24.0f, 24.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mEscTexture.id, 0.0f, Colour(255, 255, 255, 255));

	mGameOverSpriteBatch.addSprite(glm::vec2(mScreenWidth * 0.342f, mScreenHeight * 0.2f), glm::vec2(30.0f, 24.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mStartTexture.id, 0.0f, Colour(255, 255, 255, 255));

	mGameOverSpriteBatch.end();
	mGameOverSpriteBatch.renderBatches();

	//Text buffer
	char buffer[128];

	mGameOverTextBatch.begin();

	//Text colours
	Colour defaultColour(255, 255, 255, 255);
	Colour timeColour(128, 128, 255, 255);
	Colour killsColour(255, 160, 64, 255);
	Colour aggressionColour(255, 0, 0, 255);
	Colour difficultyColour(37, 226, 0, 255);
	Colour scoreColour(255, 255, 0, 255);

	//Game Over text formatting
	sprintf_s(buffer, "Game Over");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.85f),
		glm::vec2(1.0f), 0.0f, scoreColour, Justification::MIDDLE);

	sprintf_s(buffer, "Time: ");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.8f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::LEFT);

	sprintf_s(buffer, "%d seconds", roundTime);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.8f),
		glm::vec2(1.0f), 0.0f, timeColour, Justification::RIGHT);

	sprintf_s(buffer, "Kills: ");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.75f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::LEFT);

	sprintf_s(buffer, "%d ninjas", kills);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.75f),
		glm::vec2(1.0f), 0.0f, killsColour, Justification::RIGHT);

	sprintf_s(buffer, "Aggression: ");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.7f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::LEFT);

	sprintf_s(buffer, "%.2f Kills / 5 seconds", aggression);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.7f),
		glm::vec2(1.0f), 0.0f, aggressionColour, Justification::RIGHT);

	sprintf_s(buffer, "Difficulty: ");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.65f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::LEFT);

	sprintf_s(buffer, "%dx", difficulty);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.65f),
		glm::vec2(1.0f), 0.0f, difficultyColour, Justification::RIGHT);

	sprintf_s(buffer, "Final Score: ");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::LEFT);

	sprintf_s(buffer, "%d", roundTime);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.51f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, timeColour, Justification::MIDDLE);

	sprintf_s(buffer, "x");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.54f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::MIDDLE);

	sprintf_s(buffer, "%.2f", aggression);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.57f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, aggressionColour, Justification::MIDDLE);

	sprintf_s(buffer, "x");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.6f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::MIDDLE);

	sprintf_s(buffer, "%d", difficulty);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.63f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, difficultyColour, Justification::MIDDLE);

	sprintf_s(buffer, "=");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.66f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, defaultColour, Justification::MIDDLE);

	sprintf_s(buffer, "%d", score);
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, scoreColour, Justification::RIGHT);

	//Highscores
	sprintf_s(buffer, "Highscores");
	mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.5f),
		glm::vec2(1.0f), 0.0f, scoreColour, Justification::MIDDLE);

	float widthMod = 0.0f;
	float heightMod = 0.45f;

	for (int i = 0; i < highscores.getNumScores(); i++)
	{
		//Highlight your score
		Colour colour;
		if (i == rank)
		{
			colour = scoreColour;
		}
		else
		{
			colour = defaultColour;
		}

		sprintf_s(buffer, "%d.", i + 1);
		mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f + widthMod, mScreenHeight * heightMod),
			glm::vec2(1.0f), 0.0f, colour, Justification::MIDDLE);

		sprintf_s(buffer, "%d", highscores.getScore(i));
		mMenuFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.45f + widthMod, mScreenHeight * heightMod),
			glm::vec2(1.0f), 0.0f, colour, Justification::MIDDLE);

		heightMod -= 0.05f;

		if ((i + 1) % 5 == 0)
		{
			widthMod += mScreenWidth * 0.25f;
			heightMod = 0.45f;
		}
	}

	sprintf_s(buffer, "/");
	mSmallFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.335f, mScreenHeight * 0.2f),
		glm::vec2(1.0f), 0.0f, defaultColour);

	sprintf_s(buffer, "- Menu");
	mSmallFont->draw(mGameOverTextBatch, buffer, glm::vec2(mScreenWidth * 0.36f, mScreenHeight * 0.2f),
		glm::vec2(1.0f), 0.0f, defaultColour);

	mGameOverTextBatch.end();
	mGameOverTextBatch.renderBatches();
}

//Draw the HUD using the HUD camera - currently just shows fps
void GraphicsManager::drawHUD(const float fps, const float roundTime, const int kills, const Player* player)
{
	mHUDCamera.updateCamera();

	char buffer[128];

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//HUD text colours
	Colour shadowColour(0, 0, 0, 255);
	Colour defaultColour(255, 255, 255, 255);
	Colour healthColour(252, 3, 3, 255);
	Colour timeColour(128, 128, 255, 255);
	Colour killsColour(145, 26, 36, 255);

	mHUDSpriteBatch.begin();

	if (player->getDamaged())
	{
		mHUDSpriteBatch.addSprite(glm::vec2(0.0f, 0.0f), glm::vec2(mScreenWidth, mScreenHeight), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 
			player->getDamagedTexture().id, 0.0f, Colour(255, 255, 255, player->getDamagedAlpha()));
	}

	mHUDSpriteBatch.addSprite(glm::vec2(20.0f, mScreenHeight - 60.0f), glm::vec2(54.0f, 48.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mHealthTexture.id, 0.0f, Colour(255, 255, 255, 255));

	mHUDSpriteBatch.addSprite(glm::vec2(23.0f, mScreenHeight - 110.0f), glm::vec2(48.0f, 48.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mTimeTexture.id, 0.0f, Colour(255, 255, 255, 255));

	mHUDSpriteBatch.addSprite(glm::vec2(8.5f, mScreenHeight - 160.0f), glm::vec2(77.0f, 48.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		mKillsTexture.id, 0.0f, Colour(255, 255, 255, 255));

	//Health shadow
	sprintf_s(buffer, "%d", player->getHealth());
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(93.0f, mScreenHeight - 63.0f),
		glm::vec2(1.0f), 0.0f, shadowColour);

	//Add player health to the HUD buffer
	sprintf_s(buffer, "%d", player->getHealth());
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(90.0f, mScreenHeight - 60.0f),
		glm::vec2(1.0f), 0.0f, healthColour);

	//Time shadow
	sprintf_s(buffer, "%.0f", roundTime);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(93.0f, mScreenHeight - 113.0f),
		glm::vec2(1.0f), 0.0f, shadowColour);

	//Add time to the HUD buffer
	sprintf_s(buffer, "%.0f", roundTime);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(90.0f, mScreenHeight - 110.0f),
		glm::vec2(1.0f), 0.0f, timeColour);

	//Kills shadow
	sprintf_s(buffer, "%d", kills);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(93.0f, mScreenHeight - 163.0f),
		glm::vec2(1.0f), 0.0f, shadowColour);

	//Add number of kills to the HUD buffer
	sprintf_s(buffer, "%d", kills);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(90.0f, mScreenHeight - 160.0f),
		glm::vec2(1.0f), 0.0f, killsColour);

	//Add the fps to the HUD buffer
	//sprintf_s(buffer, "FPS: %.1f", fps);
	//Add the buffer to the HUD
	//mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(mScreenWidth - 10.0f, mScreenHeight * 0.97f),
	//	glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	//Sort the sprite batch and create render batches
	mHUDSpriteBatch.end();

	//Render the HUD
	mHUDSpriteBatch.renderBatches();
}

void GraphicsManager::drawGame(Player* player, std::vector<Box*>& boxEntities,
	std::vector<Ground*>& groundEntities, std::vector<Enemy*>& enemyEntities)
{
	//Update cameras
	if (player->getHealth() > 0)
	{
		mWorldCamera.setPosition(glm::vec2(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y));
	}
	mWorldCamera.updateCamera();

	const glm::mat4 cameraMatrix = mWorldCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//SpriteBatch functions
	//begin sorts by texture by default
	mEntitySpriteBatch.begin(FRONT_TO_BACK);

	//Add the boxes to the entity sprite batch
	for each (Box* b in boxEntities)
	{
		b->add(mEntitySpriteBatch, mWorldCamera);
	}

	//Add the ground to the entity sprite batch
	for each (Ground* g in groundEntities)
	{
		g->add(mEntitySpriteBatch, mWorldCamera);
	}

	//Add the enemies to the entity sprite batch
	for each (Enemy* e in enemyEntities)
	{
		e->add(mEntitySpriteBatch, mWorldCamera);
	}

	//Add the player the entity sprite batch
	player->add(mEntitySpriteBatch, mWorldCamera);

	//Sort the sprite batch and create render batches
	mEntitySpriteBatch.end();

	//Render the batches
	mEntitySpriteBatch.renderBatches();
}

void GraphicsManager::translateCamera(const glm::vec2 translation)
{
	mWorldCamera.setPosition(mWorldCamera.getPosition() + translation);
}

void GraphicsManager::setCameraScale(const float scale)
{
	mWorldCamera.setScale(mWorldCamera.getScale() + scale);
}
