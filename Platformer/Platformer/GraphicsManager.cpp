#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() 
{
}

GraphicsManager::~GraphicsManager()
{
	delete mHUDFont;

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
	mWorldCamera.setScale(32.0f);
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

	//Load the HUD front
	mHUDFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 32);

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
void GraphicsManager::drawHUD(const float fps)
{
	char buffer[128];

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mHUDSpriteBatch.begin();

	//Add the fps to the HUD buffer
	sprintf_s(buffer, "FPS: %.2f", fps);

	//Add the buffer to the HUD
	mHUDFont->draw(mHUDSpriteBatch, buffer, glm::vec2(10, mScreenHeight - 40), 
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	//Sort the sprite batch and create render batches
	mHUDSpriteBatch.end();

	//Render the HUD
	mHUDSpriteBatch.renderBatches();
}

void GraphicsManager::updateGraphics(const float fps, Player* player, std::vector<Box*>& boxEntities,
	std::vector<Ground*>& groundEntities, std::vector<Enemy*>& enemyEntities, std::vector<Marker*>& markerEntities)
{
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

	//Add the player the entity sprite batch
	player->add(mEntitySpriteBatch, mWorldCamera);

	//Sort the sprite batch and create render batches
	mEntitySpriteBatch.end();

	//Render the batches
	mEntitySpriteBatch.renderBatches();

	//Draw the HUD
	drawHUD(fps);

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
