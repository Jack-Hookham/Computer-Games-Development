#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <map>

#include "SoundEffect.h"
#include "Music.h"

//Similar to ResourceManager and TextureCache
//Stores previously loaded sound effects and music files in two different maps
//When trying to load a sound file it checks the map to see if the file has already been used
//if it hasn't it adds it to the map
//if it has it loads it from the map

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool initAudio();

	SoundEffect loadSoundEffect(const std::string& path);
	Music loadMusic(const std::string& path);

	void setVolume(int volume);

private:
	//Log for AudioManager
	void log(const std::string text);

	//Whether or not the audio manager has already been initialised
	bool mIsInitialised;

	std::map<std::string, Mix_Chunk*> mSoundEffectMap;
	std::map<std::string, Mix_Music*> mMusicMap;
};

