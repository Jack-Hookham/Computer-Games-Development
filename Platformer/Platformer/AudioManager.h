#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>
#include <map>

class SoundEffect
{
public:
	//Give AudioManager access to mChunk
	friend class AudioManager;
	
	//n = number of times to loop
	//n == -1 means loop forever
	bool play(int n = 0);

private:
	Mix_Chunk* mChunk = NULL;
};

class Music
{
public:
	friend class AudioManager;

	//n = number of times to loop
	//n == -1 means loop forever
	void play(int n = 1);

	static void stop();

	static void resume();

	static void pause();

private:
	//Give AudioManager access to mMusic
	Mix_Music* mMusic = NULL;
};

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	bool init();
	void destroy();

	SoundEffect loadSoundEffect(const std::string& path);
	Music loadMusic(const std::string& path);

private:
	//Log for AudioManager
	void log(const std::string text);

	//Whether or not the audio manager has already been initialised
	bool mIsInitialised;

	std::map<std::string, Mix_Chunk*> mSoundEffectMap;
	std::map<std::string, Mix_Music*> mMusicMap;
};

