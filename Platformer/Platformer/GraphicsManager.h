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
#include "Enemy.h"
#include "Marker.h"
#include "Highscores.h"

class GraphicsManager
{

public:
	GraphicsManager();
	~GraphicsManager();

	//Initialise SDL and create the game window
	bool initGraphics(const int screenWidth, const int screenHeight);

	void clearBuffers();
	void swapBuffers();

	//Update the graphics for the current frame
	void drawGame(Player* player, std::vector<Box*>& boxEntities,
		std::vector<Ground*>& groundEntities, std::vector<Enemy*>& enemyEntities);

	void drawHUD(const float fps, const float roundTime, const int kills, const Player* player);

	void drawMenu(Texture& menuTexture);
	void drawGameOver(const Texture& gameOverTexture, const int roundTime, const int kills, const float aggression,
		const float difficulty, const int score, Highscores& highscores, const int rank);

	void translateCamera(const glm::vec2 translation);
	void setCameraScale(const float scale);

	Camera getCamera() const { return mWorldCamera; }
	Window getWindow() const { return mWindow; }

private:
	//Log for GraphicsManager
	void log(const std::string text);

	//Initialise the shaders
	void initShaders();

	//Screen dimensions
	int mScreenWidth;
	int mScreenHeight;

	//The window to render to
	Window mWindow;

	//Camera for the world
	Camera mWorldCamera;

	//Camera for the HUD
	Camera mHUDCamera;

	//Entity sprite batch
	SpriteBatch mEntitySpriteBatch;

	//HUD sprite batch
	SpriteBatch mHUDSpriteBatch;

	//Menu sprite batches
	SpriteBatch mGameOverSpriteBatch;
	SpriteBatch mGameOverTextBatch;

	//Fonts
	SpriteFont* mHUDFont;
	SpriteFont* mMenuFont;

	//Shader for the engine
	Shader mTextureShader;

	const float DEFAULT_CAMERA_SCALE = 32.0f;
};

