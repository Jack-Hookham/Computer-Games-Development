#pragma once

#include <string>

#include "TextureCache.h"

//Static class to manage texture loading
//The texture cache stores all of the textures that are used in a map
//So if the engine tries to load an already used texture the getTexture function simply retrieves it from the map
//If the texture hasn't been loaded before then it gets added to the map
//This is similar to how the AudioManager works

class ResourceManager
{
public:
	static Texture getTexture(std::string path);

private:
	static TextureCache mTextureCache;
};