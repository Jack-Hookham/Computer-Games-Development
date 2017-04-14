#include "TextureCache.h"

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

//Get a texture
//If the texture doesn't already exist add it to the map
GLTexture TextureCache::getTexture(std::string path)
{
	std::map<std::string, GLTexture>::iterator mapIterator = mTextureMap.find(path);

	//if texture isn't in the map
	if (mapIterator == mTextureMap.end())
	{
		//Load texture
		GLTexture newTexture = ImageManager::loadTexture(path);
		//Add texture to map
		mTextureMap.insert(make_pair(path, newTexture));

		std::cout << "Loaded new texture\n";

		return newTexture;
	}
	std::cout << "Loaded cached texture\n";
	return mapIterator->second;
}
