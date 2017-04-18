#pragma once

#include <string>

#include "TextureCache.h"

class ResourceManager
{
public:
	static Texture getTexture(std::string path);
	//static Music getMusic(std::string path);
	//static SoundEffect getSoundEffect(std::string path);

private:
	static TextureCache mTextureCache;
	//static AudioCache mAudioCache;
};

