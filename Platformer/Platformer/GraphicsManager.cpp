#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(Player* player,  const int screenWidth, const int screenHeight) 
	: mPlayer(player), mScreenWidth(screenWidth), mScreenHeight(screenHeight)
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
}

bool GraphicsManager::initGraphics()
{
	//Initialization flag
	bool success = true;

	//Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		const std::string text = "SDL could not Initialise! SDL Error: " + std::string(SDL_GetError());
		log(text);
		success = false;
	}
	else
	{
		if (!mWindow.createWindow("Platformer", mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL))
		{
			log("Failed to create window");
			success = false;
		}
		else
		{
			//Initialise the camera
			mCamera.initCamera(mScreenWidth, mScreenHeight);

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
				success = false;
			}

			//Initialise SDL_ttf
			if (TTF_Init() == -1)
			{
				const std::string text = "SDL_ttf could not initialise! SDL_ttf Error: " + std::string(TTF_GetError());
				log(text);
				success = false;
			}
		}
	}

	//Initialise text textures
	//mTextTexture = new Texture(mRenderer);
	//mPromptTextTexture = new Texture(mRenderer);
	//mTimeTextTexture = new Texture(mRenderer);
	//mFPSTextTexture = new Texture(mRenderer);

	if (!loadMedia())
	{
		log("Failed to load media!");
	}
	else
	{
		log("Media successfully loaded");
	}

	initShaders();

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
		mSprites.push_back(new Sprite());
		mSprites.back()->init(-1.0f, -1.0f, mScreenWidth / 2, mScreenWidth / 2, "../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

		mSprites.push_back(new Sprite());
		mSprites.back()->init(mScreenWidth / 2, -1.0f, mScreenWidth / 2, mScreenWidth / 2, "../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
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

void GraphicsManager::updateGraphics(Timer timer, float avgFPS, float timeMod)
{
	mCamera.updateCamera();

	//cout << timeMod << endl;
	//Set depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mColourShader.use();
	glActiveTexture(GL_TEXTURE0);

	GLuint textureLocation = mColourShader.getUniformLocation("sampler");
	glUniform1i(textureLocation, 0);
	
	GLuint timeLocation = mColourShader.getUniformLocation("timeMod");
	glUniform1f(timeLocation, timeMod);

	GLuint projMatrixLocation = mColourShader.getUniformLocation("projMatrix");
	glm::mat4 cameraMatrix = mCamera.getCamerMatrix();
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (int i = 0; i < mSprites.size(); i++)
	{
		mSprites[i]->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	mColourShader.unuse();

	//Swap buffer and draw everything
	mWindow.swapWindow();

	////Clear screen
	//SDL_SetRenderDrawColor(mRenderer, 0x88, 0x88, 0x88, 0xFF);
	//SDL_RenderClear(mRenderer);

	//SDL_Color textColour = { 0, 0, 60, 255 };

	//////Render red filled quad
	////SDL_Rect fillRect = { mScreenWidth / 4, mScreenHeight / 4, mScreenWidth / 2, mScreenHeight / 2 };
	////SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);
	////SDL_RenderFillRect(mRenderer, &fillRect);

	//////Render green outlined quad
	////SDL_Rect outlineRect = { mScreenWidth / 6, mScreenHeight / 6, mScreenWidth * 2 / 3, mScreenHeight * 2 / 3 };
	////SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
	////SDL_RenderDrawRect(mRenderer, &outlineRect);

	//////Draw blue horizontal line
	////SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0xFF, 0xFF);
	////SDL_RenderDrawLine(mRenderer, 0, mScreenHeight / 2, mScreenWidth, mScreenHeight / 2);

	//////Draw vertical line of yellow dots
	////SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	////for (int i = 0; i < mScreenHeight; i += 4)
	////{
	////	SDL_RenderDrawPoint(mRenderer, mScreenWidth / 2, i);
	////}

	////Render player
	//SDL_Rect playerRect = { mPlayer->getPosition().getX(), mPlayer->getPosition().getY(),
	//						mPlayer->getWidth() * WORLD_TO_SCREEN, mPlayer->getHeight() * WORLD_TO_SCREEN };
	//SDL_SetRenderDrawColor(mRenderer, 0x66, 0x00, 0xFF, 0xFF);
	//SDL_RenderFillRect(mRenderer, &playerRect);
	//
	////Render text
	//mTextTexture->render((mScreenWidth - mTextTexture->getWidth()) / 2, (mScreenHeight - mTextTexture->getHeight()) / 3);
	//mPromptTextTexture->render((mScreenWidth - mPromptTextTexture->getWidth()) / 2, 0);

	//timeText.str("");
	//timeText << "Milliseconds since start time " << timer.getTicks();

	////Update time texture with new time
	//if (!mTimeTextTexture->loadFromRenderedText(mMainFont, timeText.str().c_str(), textColour))
	//{
	//	log("Unable to render time texture!");
	//}

	//mTimeTextTexture->render((mScreenWidth - mPromptTextTexture->getWidth()) / 2, (mScreenHeight - mPromptTextTexture->getHeight()) / 2);

	//fpsText.str("");
	//fpsText << std::setprecision(2) << "avgFPS: " << avgFPS;

	////Update fps texture with new fps
	//if (!mFPSTextTexture->loadFromRenderedText(mMainFont, fpsText.str().c_str(), textColour))
	//{
	//	log("Unable to render fps texture!");
	//}

	//mFPSTextTexture->render((mScreenWidth - mFPSTextTexture->getWidth()) / 2, (mScreenHeight - mFPSTextTexture->getHeight()) / 1.5);

	////Update screen
	//SDL_RenderPresent(mRenderer);
}

void GraphicsManager::translateCamera(glm::vec2 translation)
{
	mCamera.setPosition(mCamera.getPosition() + translation);
}

void GraphicsManager::setCameraScale(float scale)
{
	mCamera.setScale(mCamera.getScale() + scale);
}
