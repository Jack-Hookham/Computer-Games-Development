#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <map>

#include "SoundEffect.h"
#include "Music.h"

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool init();

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

