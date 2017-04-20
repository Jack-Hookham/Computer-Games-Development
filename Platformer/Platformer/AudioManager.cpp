#include "AudioManager.h"

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	if (mIsInitialised)
	{
		mIsInitialised = false;

		//Free all chunks
		for (std::pair<const std::string, Mix_Chunk*>& mapIterator : mSoundEffectMap)
		{
			Mix_FreeChunk(mapIterator.second);
		}

		//Free all music
		for (std::pair<const std::string, Mix_Music*>& mapIterator : mMusicMap)
		{
			Mix_FreeMusic(mapIterator.second);
		}

		//Clear the maps
		mSoundEffectMap.clear();
		mMusicMap.clear();

		//Quit SDL system
		Mix_CloseAudio();
		Mix_Quit();
	}
}

bool AudioManager::initAudio()
{
	log("Initialising");

	//Initilisation flag
	bool success = true;

	if (mIsInitialised)
	{
		log("Tried to initialise AudioManager more than once");
		return success = true;
	}

	//If Mix_Init fails
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
	{
		log("Mix_Init failed: " + std::string(Mix_GetError()));
		success = false;
	}

	//Initialise OpenAudio with default frequency and format
	//channels = 2, chunksize = 1024
	//If Mix_OpenAudio fails
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		log("Mix_OpenAudio failed: " + std::string(Mix_GetError()));
		success = false;
	}
	
	//Set initialised bool to true
	mIsInitialised = true;

	if (success)
	{
		log("Initialised");
	}

	return success;
}

SoundEffect AudioManager::loadSoundEffect(const std::string& path)
{
	//Iterator for the sound effect map
	std::map<std::string, Mix_Chunk*>::iterator mapIterator = mSoundEffectMap.find(path);

	SoundEffect soundEffect;

	//Search for the sound effect in the cache
	if (mapIterator == mSoundEffectMap.end())
	{
		//Failed to find add it
		Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

		//If Mix_LoadWAV fails
		if (chunk == NULL)
		{
			log("Mix_LoadWAV failed: " + std::string(Mix_GetError()));
		}
		else
		{
			//Add the chunk
			soundEffect.mChunk = chunk;
			mSoundEffectMap[path] = chunk;
		}
	}
	else
	{
		//Already cached
		soundEffect.mChunk = mapIterator->second;
	}

	return soundEffect;
}

//Load a music file from the given file path
Music AudioManager::loadMusic(const std::string& path)
{
	//Iterator for the music map
	std::map<std::string, Mix_Music*>::iterator mapIterator = mMusicMap.find(path);

	Music music;

	//Search for the sound effect in the cache
	if (mapIterator == mMusicMap.end())
	{
		//Failed to find add it
		Mix_Music* mixMusic = Mix_LoadMUS(path.c_str());

		//If Mix_LoadWAV fails
		if (mixMusic == NULL)
		{
			log("Mix_LoadMUS failed: " + std::string(Mix_GetError()));
		}
		else
		{
			//Add the chunk
			music.mMusic = mixMusic;
			mMusicMap[path] = mixMusic;
		}
	}
	else
	{
		//Already cached
		music.mMusic = mapIterator->second;
	}

	return music;
}

//Set the audio volume (0 to 128)
void AudioManager::setVolume(int volume)
{
	Mix_Volume(-1, volume);
	Mix_VolumeMusic(volume);
}

void AudioManager::log(const std::string text)
{
	std::cout << "[AudioManager] " << text << std::endl;
}
