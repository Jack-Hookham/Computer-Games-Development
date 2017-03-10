#pragma once
#include "GameManager.h"


class GraphicsManager
{

public:
	GraphicsManager();
	~GraphicsManager();

	//initialise SDL and create window
	bool initGraphics();

		//Loads media needed for the game
	bool loadMedia();

	SDL_Texture* loadTexture(std::string path);

	//Loads individual image as texture

private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;
};

