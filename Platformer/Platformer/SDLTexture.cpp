/*
* SDLTexture class based on Lazy Foo SDL tutorials
* http://lazyfoo.net/tutorials/SDL/
*/

#include "SDLTexture.h"

SDLTexture::SDLTexture(SDL_Renderer* renderer)
{
	//Initialize
	mRenderer = renderer;
	mSDLTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

SDLTexture::~SDLTexture()
{
	//Free renderer if it exists
	if (mRenderer != NULL)
	{
		SDL_DestroyRenderer(mRenderer);
		mRenderer = NULL;
	}

	//Free SDLTexture
	freeSDLTexture();
}

bool SDLTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting SDLTexture
	freeSDLTexture();

	//The final SDLTexture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create SDLTexture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create SDLTexture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mSDLTexture = newTexture;
	return mSDLTexture != NULL;
}

bool SDLTexture::loadFromRenderedText(TTF_Font* font, std::string SDLTextureText, SDL_Color textColor)
{
	//Get rid of preexisting SDLTexture
	freeSDLTexture();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, SDLTextureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create SDLTexture from surface pixels
		mSDLTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
		if (mSDLTexture == NULL)
		{
			printf("Unable to create SDLTexture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mSDLTexture != NULL;
}

void SDLTexture::freeSDLTexture()
{
	//Free SDLTexture if it exists
	if (mSDLTexture != NULL)
	{
		SDL_DestroyTexture(mSDLTexture);
		mSDLTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void SDLTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate SDLTexture
	SDL_SetTextureColorMod(mSDLTexture, red, green, blue);
}

void SDLTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mSDLTexture, blending);
}

void SDLTexture::setAlpha(Uint8 alpha)
{
	//Modulate SDLTexture alpha
	SDL_SetTextureAlphaMod(mSDLTexture, alpha);
}

void SDLTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(mRenderer, mSDLTexture, clip, &renderQuad, angle, center, flip);
}

int SDLTexture::getWidth()
{
	return mWidth;
}

int SDLTexture::getHeight()
{
	return mHeight;
}