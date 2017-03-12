#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(Player* player, const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
	mPlayer = player;
	this->SCREEN_WIDTH = SCREEN_WIDTH;
	this->SCREEN_HEIGHT = SCREEN_HEIGHT;

	
}

GraphicsManager::~GraphicsManager()
{
	//Destroy window	
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;
	mRenderer = NULL;
}

bool GraphicsManager::initGraphics()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		const std::string text = "SDL could not initialize! SDL Error: " + std::string(SDL_GetError());
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
				//Initialize renderer color
				SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					const std::string text = "SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError());
					log(text);
					success = false;
				}
			}
		}
	}

	return success;
}

bool GraphicsManager::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
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

void GraphicsManager::updateGraphics()
{
	//Clear screen
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	//Render red filled quad
	SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(mRenderer, &fillRect);

	//Render green outlined quad
	SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(mRenderer, &outlineRect);

	//Draw blue horizontal line
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawLine(mRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

	//Draw vertical line of yellow dots
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < SCREEN_HEIGHT; i += 4)
	{
		SDL_RenderDrawPoint(mRenderer, SCREEN_WIDTH / 2, i);
	}

	//Render player
	SDL_Rect playerRect = { mPlayer->getPosition().getX(), mPlayer->getPosition().getY(),
							mPlayer->getWidth() * WORLD_TO_SCREEN, mPlayer->getHeight() * WORLD_TO_SCREEN };
	SDL_SetRenderDrawColor(mRenderer, 0x66, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(mRenderer, &playerRect);
	//mPlayer

	//Update screen
	SDL_RenderPresent(mRenderer);
}
