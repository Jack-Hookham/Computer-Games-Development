#pragma once

#include <GL/glew.h>
#include <map>

#include "ImageManager.h"
#include "Texture.h"

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

