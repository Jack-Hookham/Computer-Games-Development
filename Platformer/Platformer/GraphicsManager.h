#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <cmath>
#include <sstream>

//#include "PlatformerCommon.h"
#include "Player.h"
#include "Texture.h"
//#include "GameManager.h"

class GraphicsManager
{

public:
	GraphicsManager(Player* player, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
	~GraphicsManager();

	//initialise SDL and create window
	bool initGraphics();

	//Loads media needed for the game
	bool loadMedia();

	//Loads individual image as texture
	SDL_Texture* loadTexture(std::string path);

	//Log for GraphicsManager
	void log(const std::string text);

	void updateGraphics(Uint32 startTime);

private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	const float WORLD_TO_SCREEN = 10.0f;

	//The window we'll be rendering to
	SDL_Window* mWindow = NULL;

	//The window renderer
	SDL_Renderer* mRenderer = NULL;

	Player* mPlayer;

	TTF_Font* mMainFont;
	Texture* mTextTexture;
	Texture* mPromptTextTexture;
	Texture* mTimeTextTexture;

	//In memory text stream
	std::stringstream timeText;
};

