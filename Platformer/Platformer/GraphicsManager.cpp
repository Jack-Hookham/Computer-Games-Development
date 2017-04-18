#include "GraphicsManager.h"

GraphicsManager::GraphicsManager() 
{
}

GraphicsManager::~GraphicsManager()
{
	//Free loaded images
	//mTextTexture->freeTexture();
	//mPromptTextTexture->freeTexture();
	//mTimeTextTexture->freeTexture();
	//mFPSTextTexture->freeTexture();

	//Free font
	//TTF_CloseFont(mMainFont);
	//mMainFont = NULL;

	//Destroy window	
	//SDL_DestroyRenderer(mRenderer);
	//SDL_DestroyWindow(mWindow);
	//mWindow = NULL;
	//mRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	delete mSpriteFont;
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
		return success = false;
	}

	if (!mWindow.createWindow("Platformer", mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))
	{
		log("Failed to create window");
		return success = false;
	}

	//Initialise the cameras
	mWorldCamera.initCamera(mScreenWidth, mScreenHeight);
	mWorldCamera.setScale(16.0f);
	mHUDCamera.initCamera(mScreenWidth, mScreenHeight);
	//Offset the hud camera to align 0, 0 with the bottom left corner
	mHUDCamera.setPosition(glm::vec2(mScreenWidth / 2, mScreenHeight / 2));

	//Initialise OpenGL
	//if (!initGL())
	//{
	//	log("Unable to initialize OpenGL!\n");
	//	success = false;
	//}
	//Initialise renderer color
	//SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialise PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		const std::string text = "SDL_image could not Initialise! SDL_image Error: " + std::string(IMG_GetError());
		log(text);
		return success = false;
	}

	//Initialise SDL_ttf
	if (TTF_Init() == -1)
	{
		const std::string text = "SDL_ttf could not initialise! SDL_ttf Error: " + std::string(TTF_GetError());
		log(text);
		return success = false;
	}

	//Initialise text textures
	//mTextTexture = new Texture(mRenderer);
	//mPromptTextTexture = new Texture(mRenderer);
	//mTimeTextTexture = new Texture(mRenderer);
	//mFPSTextTexture = new Texture(mRenderer);

	initShaders();

	//Generate VBO and VAO to initialise the sprite batch
	mEntitySpriteBatch.bufferData();
	mHUDSpriteBatch.bufferData();

	//Initialise sprite font
	mSpriteFont = new SpriteFont("../res/fonts/arial_narrow_7/arial_narrow_7.ttf", 32);

	if (!loadMedia())
	{
		log("Failed to load media!");
	}
	else
	{
		log("Media successfully loaded");
	}

	return success;
}

bool GraphicsManager::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	//mMainFont = TTF_OpenFont("../res/fonts/lazy.ttf", 28);
	//if (mMainFont == NULL)
	//{
	//	const std::string text = "Failed to load font! SDL_ttf Error: " + std::string(TTF_GetError());
	//	log(text);
	//	success = false;
	//}
	//else
	{
		//Set text colour as black
		//SDL_Color textColour = { 0, 0, 0, 255 };

		//if (!mTextTexture->loadFromRenderedText(mMainFont, "This text is a texture", textColour))
		//{
		//	log("Failed to render text texture!");
		//	success = false;
		//}

		//if (!mPromptTextTexture->loadFromRenderedText(mMainFont, "Press Enter to Reset Start Time.", textColour))
		//{
		//	log("Failed to render text texture!");
		//	success = false;
		//}

		//mPlayerTexture = ImageManager::loadTexture("../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

		//mSprites.push_back(new Sprite());
		//mSprites.back()->init(0.0f, 0.0f, mScreenWidth / 2, mScreenWidth / 2, "../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

		//mSprites.push_back(new Sprite());
		//mSprites.back()->init(mScreenWidth / 2, 0.0f, mScreenWidth / 2, mScreenWidth / 2, "../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	}

	return success;
}

void GraphicsManager::log(const std::string text)
{
	std::cout << "[GraphicsManager] " << text << std::endl;
}

void GraphicsManager::initShaders()
{
	mColourShader.compileShaders("../res/shaders/colourVert.glsl", "../res/shaders/colourFrag.glsl");
//	mColourShader.compileShaders("../res/shaders/colorShading.vert", "../res/shaders/colorShading.frag");

	mColourShader.addAttribute("position");
	mColourShader.addAttribute("colour");
	mColourShader.addAttribute("texCoord");
	mColourShader.linkShaders();
}

void GraphicsManager::drawHUD(float avgFPS)
{
	char buffer[256];

	glm::mat4 cameraMatrix = mHUDCamera.getCamerMatrix();
	GLuint projMatrixLocation = mColourShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	mHUDSpriteBatch.begin();

	sprintf_s(buffer, "FPS: %f", avgFPS);

	mSpriteFont->draw(mHUDSpriteBatch, buffer, glm::vec2(10, 10), 
		glm::vec2(1.0f), 0.0f, Colour(255, 255, 255, 255));

	mHUDSpriteBatch.end();
	mHUDSpriteBatch.renderBatches();
}

//SDL_Texture* GraphicsManager::loadTexture(std::string path)
//{
//	//The final texture
//	SDL_Texture* newTexture = NULL;
//
//	//Load image at specified path
//	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
//	if (loadedSurface == NULL)
//	{
//		const std::string text = "Unable to load image " + path + "! SDL_image Error: " + std::string(IMG_GetError());
//		log(text);
//	}
//	else
//	{
//		//Create texture from surface pixels
//		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
//		newTexture = NULL;
//		if (newTexture == NULL)
//		{
//			const std::string text = "Unable to create texture from " + path + "! SDL Error: " + std::string(SDL_GetError());
//			log(text);
//		}
//
//		//Get rid of old loaded surface
//		SDL_FreeSurface(loadedSurface);
//	}
//
//	return newTexture;
//}

void GraphicsManager::updateGraphics(float avgFPS, std::vector<Entity>& entities)
{
	mWorldCamera.updateCamera();
	mHUDCamera.updateCamera();

	//Set depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mColourShader.use();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mColourShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);

	glm::mat4 cameraMatrix = mWorldCamera.getCamerMatrix();
	GLuint projMatrixLocation = mColourShader.getUniformLocation("projMatrix");
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//SpriteBatch functions
	//begin sorts by texture by default
	mEntitySpriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 texCoords(0.0f, 0.0f, 1.0f, 1.0f);
	//GLTexture texture = ResourceManager::getTexture("../res/textures/platform_tutorial/bricks_top.png");
	//GLTexture texture = ResourceManager::getTexture("../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Colour colour(255, 255, 255, 255);

	//for (int i = 0; i < 10; i++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		mEntitySpriteBatch.draw(pos + glm::vec4(50 * i, 50 * j, 0, 0), texCoords, texture.id, 0.0f, colour);
	//	}
	//}
	//mEntitySpriteBatch.draw(pos, texCoords, texture.id, 0.0f, colour);

	//if (mWorldCamera.cullOffScreen(glm::vec2(pos.x, pos.y), glm::vec2(pos.z, pos.w)))
	//{
	//	mEntitySpriteBatch.addQuad(pos, texCoords, texture.id, 0.0f, colour);
	//}

	//for (unsigned int i = 0; i < bullets.size(); i++)
	//{
	//	bullets[i].draw(mEntitySpriteBatch);
	//}

	//Draw all boxes
	for (Entity e : entities)
	{
		e.draw(mEntitySpriteBatch);
		//mEntitySpriteBatch.addQuad(destQuad, boxTexCoords, boxTexture.id, 0.0f, b.getColour(), b.getBody()->GetAngle());
	}

	mEntitySpriteBatch.end();
	mEntitySpriteBatch.renderBatches();

	drawHUD(avgFPS);

	glBindTexture(GL_TEXTURE_2D, 0);
	mColourShader.unuse();

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
