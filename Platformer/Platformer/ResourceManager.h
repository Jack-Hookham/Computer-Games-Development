#pragma once

#include <string>

#include "TextureCache.h"

class ResourceManager
{
public:
	static GLTexture getTexture(std::string path);

private:
	static TextureCache mTextureCache;

};

