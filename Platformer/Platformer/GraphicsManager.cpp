#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() 
{
}

GraphicsManager::~GraphicsManager()
{
	delete mHUDFont;
	delete mMenuFont;

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
	if (!mWindow.createWindow("Platformer", mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))
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
	mMenuSpriteBatch.bufferData();
	mMenuTextBatch.bufferData();

	//Load the HUD front
	mHUDFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 24);
	mMenuFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 36);

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

//Draw the HUD using the HUD camera - currently just shows fps
void GraphicsManager::drawHUD(const float fps, const float roundTime, const Player* player)
{
	char buffer[128];

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mHUDSpriteBatch.begin();

	//Add player health to the HUD buffer
	sprintf_s(buffer, "Player HP: %d", player->getHealth());
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(10.0f, mScreenHeight - 30.0f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	//Add time to the HUD buffer
	sprintf_s(buffer, "Time: %.0f", roundTime);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(10.0f, mScreenHeight - 50.0f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	//Add the fps to the HUD buffer
	sprintf_s(buffer, "FPS: %.1f", fps);
	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(mScreenWidth - 100.0f, mScreenHeight - 30.0f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	//Sort the sprite batch and create render batches
	mHUDSpriteBatch.end();

	//Render the HUD
	mHUDSpriteBatch.renderBatches();
}

void GraphicsManager::updateGraphics(const float fps, const float roundTime, Player* player, std::vector<Box*>& boxEntities,
	std::vector<Ground*>& groundEntities, std::vector<Enemy*>& enemyEntities, std::vector<Marker*>& markerEntities, 
	std::vector<Marker*>& collisionBoxEntities, std::vector<glm::vec2>& enemySpawnPositions)
{
	mScreenWidth = SDL_GetWindowSurface(mWindow.getSDLWindow())->w; 
	mScreenHeight = SDL_GetWindowSurface(mWindow.getSDLWindow())->h;

	//Update cameras
	const glm::vec2 worldCameraPos = glm::vec2(player->getBody()->GetPosition().x, player->getBody()->GetPosition().y);
	mWorldCamera.setPosition(worldCameraPos);
	mWorldCamera.updateCamera();

	mHUDCamera.updateCamera();

	//Set depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader attributes
	mTextureShader.enableAttributes();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mTextureShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

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
	
	//Add the markers to the entity sprite batch
	for each (Marker* m in markerEntities)
	{
		m->add(mEntitySpriteBatch, mWorldCamera);
	}

	for each (Marker* m in collisionBoxEntities)
	{
		m->add(mEntitySpriteBatch, mWorldCamera);
	}

	for each (glm::vec2 pos in enemySpawnPositions)
	{
		mEntitySpriteBatch.addSprite(pos, glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), collisionBoxEntities[0]->getTexture().id, 0.0f, Colour(255, 255, 255, 255));
	}

	//Add the player the entity sprite batch
	player->add(mEntitySpriteBatch, mWorldCamera);

	//Sort the sprite batch and create render batches
	mEntitySpriteBatch.end();

	//Render the batches
	mEntitySpriteBatch.renderBatches();

	//Draw the HUD
	drawHUD(fps, roundTime, player);

	//Undbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader attributes
	mTextureShader.disableAttributes();

	//Swap buffer and draw everything
	mWindow.swapWindow();
}

void GraphicsManager::drawMenu(Texture& menuTexture)
{
	mScreenWidth = SDL_GetWindowSurface(mWindow.getSDLWindow())->w;
	mScreenHeight = SDL_GetWindowSurface(mWindow.getSDLWindow())->h;

	//Reuse HUD camera for menu
	mHUDCamera.updateCamera();

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Enable the shader attributes
	mTextureShader.enableAttributes();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mTextureShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mMenuSpriteBatch.begin();

	mMenuSpriteBatch.addSprite(glm::vec2(0.0f, 0.0f), glm::vec2(mScreenWidth, mScreenHeight), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), menuTexture.id,
		0.0f, Colour(255, 255, 255, 255));

	mMenuSpriteBatch.end();
	mMenuSpriteBatch.renderBatches();	

	char buffer[128];

	mMenuTextBatch.begin();

	//Menu text
	sprintf_s(buffer, "Menu");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.8f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "Space / A - Play");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.75f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Esc - Quit");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.7f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Controls");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.6f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "Keyboard");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.55f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Controller");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.55f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	sprintf_s(buffer, "A & D -");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.5f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Left & Right Movement");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.5f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "- Left Stick");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.5f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	sprintf_s(buffer, "W -");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.45f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Jump");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.45f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "- A");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.45f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	sprintf_s(buffer, "Space -");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.4f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Sword Attack");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.4f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "- X");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.4f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	sprintf_s(buffer, "LMB -");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.35f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Ranged Attack 1");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.35f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "- B");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.35f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	sprintf_s(buffer, "RMB -");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.3f, mScreenHeight * 0.3f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	sprintf_s(buffer, "Ranged Attack 2");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.5f, mScreenHeight * 0.3f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::MIDDLE);

	sprintf_s(buffer, "- Y");
	mMenuFont->draw(mMenuTextBatch, buffer, glm::vec2(mScreenWidth * 0.7f, mScreenHeight * 0.3f),
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255), Justification::RIGHT);

	//Sort the sprite batch and create render batches
	mMenuTextBatch.end();

	//Render the HUD
	//mMenuTextBatch.renderBatches();

	//Undbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disable the shader attributes
	mTextureShader.disableAttributes();

	//Swap buffer and draw everything
	mWindow.swapWindow();
}

void GraphicsManager::translateCamera(const glm::vec2 translation)
{
	mWorldCamera.setPosition(mWorldCamera.getPosition() + translation);
}

void GraphicsManager::setCameraScale(const float scale)
{
	mWorldCamera.setScale(mWorldCamera.getScale() + scale);
}
