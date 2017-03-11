#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <cmath>

class GraphicsManager
{

public:
	GraphicsManager();
	~GraphicsManager();

	//initialise SDL and create window
	bool initGraphics();

	//Loads media needed for the game
	bool loadMedia();

	//Loads individual image as texture
	SDL_Texture* loadTexture(std::string path);

	//Log graphics errors
	void log(const std::string text);

	void updateGraphics();


private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	//The window we'll be rendering to
	SDL_Window* mWindow = NULL;

	//The window renderer
	SDL_Renderer* mRenderer = NULL;
};

