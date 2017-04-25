#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

#include "ImageManager.h"
#include "Texture.h"

//Static class to manage texture loading
//The map mTextureMap stores all of the textures that are used
//So if the engine tries to load an already used texture the getTexture function simply retrieves it from the map
//If the texture hasn't been loaded before then it gets added to the map
//This is similar to how the AudioManager works

class ResourceManager
{
public:
	//Get a texture
	//If the texture doesn't already exist add it to the map
	static Texture getTexture(const std::string path);

private:
	//Log for ResourceManager
	static void log(const std::string text);

	//Map of cached textures
	static std::map<std::string, Texture> mTextureMap;
};