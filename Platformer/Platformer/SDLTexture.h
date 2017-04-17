#pragma once
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

//SDLTexture wrapper class
class SDLTexture
{
public:
	//Initializes variables
	SDLTexture(SDL_Renderer* renderer);

	//Deallocates memory
	~SDLTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(TTF_Font* font, std::string SDLTextureText, SDL_Color textColor);

	//Deallocates SDLTexture
	void freeSDLTexture();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders SDLTexture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware SDLTexture
	SDL_Texture* mSDLTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

	//The window renderer
	SDL_Renderer* mRenderer;
};