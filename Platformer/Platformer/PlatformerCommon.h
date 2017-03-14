#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

//The window we'll be rendering to
extern SDL_Window* gWindow = NULL;

//The window renderer
extern SDL_Renderer* gRenderer = NULL;

extern TTF_Font* gFont;

//extern PhysicsManager* gPhysicsManager;
//extern GraphicsManager* gGraphicsManager;