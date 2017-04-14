#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>

#include "Player.h"
#include "Texture.h"
#include "Timer.h"
#include "Sprite.h"
#include "Shader.h"
#include "GLTexture.h"
#include "ImageManager.h"
#include "Window.h"

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

	void updateGraphics(Timer timer, float avgFPS, float timeMod);

private:
	//Log for GraphicsManager
	void log(const std::string text);

	void initShaders();

	int mScreenWidth;
	int mScreenHeight;

	const float WORLD_TO_SCREEN = 10.0f;

	//The window we'll be rendering to
	Window mWindow;

	Player* mPlayer;

	std::vector<Sprite*> mSprites;

	Shader mColourShader;
	//The window renderer
	//SDL_Renderer* mRenderer = NULL;

	//OpenGL context
	//SDL_GLContext mContext;

	//TTF_Font* mMainFont;
	//Texture* mTextTexture;
	//Texture* mPromptTextTexture;
	//Texture* mTimeTextTexture;
	//Texture* mFPSTextTexture;

	//In memory text stream
	//std::stringstream timeText;
	//std::stringstream fpsText;


};

