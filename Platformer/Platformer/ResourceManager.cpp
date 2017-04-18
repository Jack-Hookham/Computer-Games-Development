#include "ResourceManager.h"

TextureCache ResourceManager::mTextureCache;

Texture ResourceManager::getTexture(std::string path)
{
	return mTextureCache.getTexture(path);
}
