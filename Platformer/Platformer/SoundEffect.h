#pragma once

#include <SDL/SDL_mixer.h>

//Stores sound effect information
//Managed by AudioManager

class SoundEffect
{
public:
	SoundEffect();
	~SoundEffect();

	//Give AudioManager access to mChunk
	friend class AudioManager;

	//n = number of times to loop
	//n == -1 means loop forever
	bool play(int n = 0);

private:
	//Sound effect chunk
	Mix_Chunk* mChunk = NULL;
};

