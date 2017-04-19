#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() 
{
}

GraphicsManager::~GraphicsManager()
{
	delete mSpriteFont;

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

bool GraphicsManager::initGraphics(const int screenWidth, const int screenHeight)
{
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

	loadMedia();

	if (success)
	{
		log("Initialised");
	}

	return success;
}

void GraphicsManager::loadMedia()
{
	//Initialise sprite font
	mSpriteFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 32);

	//Initialise textures (cache them so that they load quickly when needed)
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box1.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box2.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box3.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box4.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box5.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box6.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box7.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box8.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box9.png");
	ResourceManager::getTexture("../res/textures/boxes_and_crates/obj_box10.png");

	//Spritesheets
	ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/run.png");
	ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/idle.png");
	ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/jump.png");
	ResourceManager::getTexture("../res/textures/ninja_adventure/spritesheet/attack.png");

	log("Media successfully loaded");
}

void GraphicsManager::log(const std::string text)
{
	std::cout << "[GraphicsManager] " << text << std::endl;
}

//Compile and link the shaders used for the engine
void GraphicsManager::initShaders()
{
	mTextureShader.compileShaders("../res/shaders/colourVert.glsl", "../res/shaders/colourFrag.glsl");

	mTextureShader.addAttribute("position");
	mTextureShader.addAttribute("colour");
	mTextureShader.addAttribute("texCoord");
	mTextureShader.linkShaders();
}

//Draw the HUD using the HUD camera
void GraphicsManager::drawHUD(float avgFPS)
{
	char buffer[256];

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mHUDSpriteBatch.begin();

	sprintf_s(buffer, "FPS: %.2f", avgFPS);

	mSpriteFont->draw(mHUDSpriteBatch, buffer, glm::vec2(10, mScreenHeight - 40), 
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	mHUDSpriteBatch.end();
	mHUDSpriteBatch.renderBatches();
}

void GraphicsManager::updateGraphics(float avgFPS, Player& player, std::vector<Box>& boxEntities, std::vector<Ground>& groundEntities)
{
	//Update cameras
	glm::vec2 worldCameraPos = glm::vec2(player.getBody()->GetPosition().x, player.getBody()->GetPosition().y);
	mWorldCamera.setPosition(worldCameraPos);
	mWorldCamera.updateCamera();

	mHUDCamera.updateCamera();

	//Set depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mTextureShader.use();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mTextureShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	glm::mat4 cameraMatrix = mWorldCamera.getCamerMatrix();
	GLuint projMatrixLocation = mTextureShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//SpriteBatch functions
	//begin sorts by texture by default
	mEntitySpriteBatch.begin();

	//Add the player the entity sprite batch
	player.add(mEntitySpriteBatch, mWorldCamera);

	//Add the boxes to the entity sprite batch
	for (Box e : boxEntities)
	{
		e.add(mEntitySpriteBatch, mWorldCamera);
	}

	//Add the ground to the entity sprite batch
	for (Ground e : groundEntities)
	{
		e.add(mEntitySpriteBatch, mWorldCamera);
	}

	//End the SpriteBatch
	mEntitySpriteBatch.end();

	//Render the sprite batches
	mEntitySpriteBatch.renderBatches();

	//Draw the HUD
	drawHUD(avgFPS);

	//Undbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	mTextureShader.unuse();

	//Swap buffer and draw everything
	mWindow.swapWindow();
}

void GraphicsManager::translateCamera(glm::vec2 translation)
{
	mWorldCamera.setPosition(mWorldCamera.getPosition() + translation);
}

void GraphicsManager::setCameraScale(float scale)
{
	mWorldCamera.setScale(mWorldCamera.getScale() + scale);
}

Camera GraphicsManager::getCamera()
{
	return mWorldCamera;
}
