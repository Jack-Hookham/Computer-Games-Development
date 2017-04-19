#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <map>

#include "SoundEffect.h"
#include "Music.h"

//Uses SDL_mixer to play audio files
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

	//Initialise the AudioManager
	bool initAudio();

	//Load a sound effect
	SoundEffect loadSoundEffect(const std::string& path);

	//Load a music file
	Music loadMusic(const std::string& path);

	//Set the volume for both music and sound effect
	void setVolume(int volume);

private:
	//Log for AudioManager
	void log(const std::string text);

	//Whether or not the audio manager has already been initialised
	bool mIsInitialised;

	//Maps for storing loaded sound effects and music files
	std::map<std::string, Mix_Chunk*> mSoundEffectMap;
	std::map<std::string, Mix_Music*> mMusicMap;
};

