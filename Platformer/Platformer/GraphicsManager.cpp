#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(Player* player,  const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
	mPlayer = player;
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	this->SCREEN_HEIGHT = SCREEN_HEIGHT;
}

GraphicsManager::~GraphicsManager()
{
	//Free loaded images
	mTextTexture->freeTexture();
	mPromptTextTexture->freeTexture();
	mTimeTextTexture->freeTexture();
	mFPSTextTexture->freeTexture();

	//Free global font
	TTF_CloseFont(mMainFont);
	mMainFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	mRenderer = NULL;

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
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		const std::string text = "SDL could not Initialise! SDL Error: " + std::string(SDL_GetError());
		log(text);
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			log("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (mWindow == NULL)
		{
			const std::string text = "Window could not be created! SDL Error: " + std::string(SDL_GetError());
			log(text);
			success = false;
		}
		else
		{
			//Create renderer for window
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
			if (mRenderer == NULL)
			{
				const std::string text = "Renderer could not be created! SDL Error: " + std::string(SDL_GetError());
				log(text);
				success = false;
			}
			else
			{
				//Initialise renderer color
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

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
					const std::string text = "SDL_ttf could not initialise! SDL_ttf Error:" + std::string(TTF_GetError());
					log(text);
					success = false;
				}
			}
			//Initialise text textures
			mTextTexture = new Texture(mRenderer);
			mPromptTextTexture = new Texture(mRenderer);
			mTimeTextTexture = new Texture(mRenderer);
			mFPSTextTexture = new Texture(mRenderer);

		}
	}

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
	mMainFont = TTF_OpenFont("../res/fonts/lazy.ttf", 28);
	if (mMainFont == NULL)
	{
		const std::string text = "Failed to load font! SDL_ttf Error: " + std::string(TTF_GetError());
		log(text);
		success = false;
	}
	else
	{
		//Set text colour as black
		SDL_Color textColour = { 0, 0, 0, 255 };

		if (!mTextTexture->loadFromRenderedText(mMainFont, "This text is a texture", textColour))
		{
			log("Failed to render text texture!");
			success = false;
		}

		if (!mPromptTextTexture->loadFromRenderedText(mMainFont, "Press Enter to Reset Start Time.", textColour))
		{
			log("Failed to render text texture!");
			success = false;
		}
	}

	return success;
}

void GraphicsManager::log(const std::string text)
{
	std::cout << "[GraphicsManager] " << text << std::endl;
}

SDL_Texture* GraphicsManager::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		const std::string text = "Unable to load image " + path + "! SDL_image Error: " + std::string(IMG_GetError());
		log(text);
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
		newTexture = NULL;
		if (newTexture == NULL)
		{
			const std::string text = "Unable to create texture from " + path + "! SDL Error: " + std::string(SDL_GetError());
			log(text);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void GraphicsManager::updateGraphics(Timer timer, float avgFPS)
{
	//Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	SDL_Color textColour = { 0, 0, 60, 255 };

	////Render red filled quad
	//SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	//SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);
	//SDL_RenderFillRect(mRenderer, &fillRect);

	////Render green outlined quad
	//SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
	//SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
	//SDL_RenderDrawRect(mRenderer, &outlineRect);

	////Draw blue horizontal line
	//SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0xFF, 0xFF);
	//SDL_RenderDrawLine(mRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

	////Draw vertical line of yellow dots
	//SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	//for (int i = 0; i < SCREEN_HEIGHT; i += 4)
	//{
	//	SDL_RenderDrawPoint(mRenderer, SCREEN_WIDTH / 2, i);
	//}

	//Render player
	SDL_Rect playerRect = { mPlayer->getPosition().getX(), mPlayer->getPosition().getY(),
							mPlayer->getWidth() * WORLD_TO_SCREEN, mPlayer->getHeight() * WORLD_TO_SCREEN };
	SDL_SetRenderDrawColor(mRenderer, 0x66, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(mRenderer, &playerRect);
	
	//Render text
	mTextTexture->render((SCREEN_WIDTH - mTextTexture->getWidth()) / 2, (SCREEN_HEIGHT - mTextTexture->getHeight()) / 3);
	mPromptTextTexture->render((SCREEN_WIDTH - mPromptTextTexture->getWidth()) / 2, 0);

	timeText.str("");
	timeText << "Milliseconds since start time " << timer.getTicks();

	//Update time texture with new time
	if (!mTimeTextTexture->loadFromRenderedText(mMainFont, timeText.str().c_str(), textColour))
	{
		log("Unable to render time texture!");
	}

	mTimeTextTexture->render((SCREEN_WIDTH - mPromptTextTexture->getWidth()) / 2, (SCREEN_HEIGHT - mPromptTextTexture->getHeight()) / 2);

	fpsText.str("");
	fpsText << "avgFPS: " << avgFPS;

	//Update fps texture with new fps
	if (!mFPSTextTexture->loadFromRenderedText(mMainFont, fpsText.str().c_str(), textColour))
	{
		log("Unable to render fps texture!");
	}

	mFPSTextTexture->render((SCREEN_WIDTH - mFPSTextTexture->getWidth()) / 2, (SCREEN_HEIGHT - mFPSTextTexture->getHeight()) / 1.5);

	//Update screen
	SDL_RenderPresent(mRenderer);
}
