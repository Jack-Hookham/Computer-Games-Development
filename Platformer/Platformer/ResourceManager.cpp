#include "ResourceManager.h"

std::map<std::string, Texture> ResourceManager::mTextureMap;

Texture ResourceManager::getTexture(const std::string path, const bool repeat)
{
	std::map<std::string, Texture>::iterator i = mTextureMap.find(path);

	//if texture isn't in the map
	if (i == mTextureMap.end())
	{
		//Load texture
		Texture newTexture = ImageManager::loadTexture(path, repeat);
		//Add texture to map
		mTextureMap.insert(make_pair(path, newTexture));

		log("Loaded new texture from: " + path);

		return newTexture;
	}
	log("Loaded cached texture from: " + path);

	//if the texture is already in the map retrieve it
	return i->second;
}

void ResourceManager::log(const std::string text)
{
	std::cout << "[ResourceManager] " << text << std::endl;
}
