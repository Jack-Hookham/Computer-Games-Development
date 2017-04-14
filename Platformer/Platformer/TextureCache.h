#pragma once

#include <map>

#include "GLTexture.h"
#include "ImageManager.h"

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture getTexture(std::string path);

private:
	std::map<std::string, GLTexture> mTextureMap;
};

