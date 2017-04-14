#include "ResourceManager.h"

TextureCache ResourceManager::mTextureCache;

GLTexture ResourceManager::getTexture(std::string path)
{
	return mTextureCache.getTexture(path);
}
