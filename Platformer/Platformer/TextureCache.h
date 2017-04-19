#pragma once

#include <GL/glew.h>
#include <map>

#include "ImageManager.h"
#include "Texture.h"

//Stores previously loaded textures in a map
//if a texture hasn't been loaded before it gets added to the map
//The TextureCache is used by the ResourceManger to get the textures it needs

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	Texture getTexture(std::string path);

private:
	//Log for TextureCache
	void log(const std::string text);

	std::map<std::string, Texture> mTextureMap;
};

