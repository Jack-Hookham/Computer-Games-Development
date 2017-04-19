#pragma once

#include <Windows.h>

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>

#include "Timer.h"
#include "Shader.h"
#include "ImageManager.h"
#include "Window.h"
#include "Camera.h"
#include "SpriteBatch.h"
#include "ResourceManager.h"
#include "SpriteFont.h"
#include "Entity.h"
#include "Player.h"
#include "Box.h"
#include "Ground.h"

class GraphicsManager
{

public:
	GraphicsManager();
	~GraphicsManager();

	//initialise SDL and create window
	bool initGraphics(const int screenWidth, const int screenHeight);

	//Loads media needed for the game
	void loadMedia();

	//Loads individual image as texture
	//SDL_Texture* loadTexture(std::string path);

	void updateGraphics(float avgFPS, Player& player, std::vector<Box>& boxEntities, std::vector<Ground>& groundEntities);

	void translateCamera(glm::vec2 translation);
	void setCameraScale(float scale);

	Camera getCamera();

private:
	//Log for GraphicsManager
	void log(const std::string text);

	//Initialise the shaders
	void initShaders();

	//Draw the HUD
	void drawHUD(float avgFPS);

	//Screen dimensions
	int mScreenWidth;
	int mScreenHeight;

	//The window to render to
	Window mWindow;

	//Camera for the world
	Camera mWorldCamera;

	//Camera for the HUD
	Camera mHUDCamera;

	Player mPlayer;

	//Entity sprite batch
	SpriteBatch mEntitySpriteBatch;

	//HUD sprite batch
	SpriteBatch mHUDSpriteBatch;

	//HUD font
	SpriteFont* mSpriteFont;

	//Shader for the engine
	Shader mTextureShader;
};

