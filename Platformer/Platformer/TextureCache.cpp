#include "TextureCache.h"

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

//Get a texture
//If the texture doesn't already exist add it to the map
Texture TextureCache::getTexture(std::string path)
{
	std::map<std::string, Texture>::iterator i = mTextureMap.find(path);

	//if texture isn't in the map
	if (i == mTextureMap.end())
	{
		//Load texture
		Texture newTexture = ImageManager::loadTexture(path);
		//Add texture to map
		mTextureMap.insert(make_pair(path, newTexture));

		log("Loaded new texture from: " + path);

		return newTexture;
	}
	log("Loaded cached texture from: " + path);
	return i->second;
}

void TextureCache::log(const std::string text)
{
	std::cout << "[TextureCache] " << text << std::endl;
}
