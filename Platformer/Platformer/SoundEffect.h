#pragma once

#include <SDL/SDL_mixer.h>

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
	Mix_Chunk* mChunk = NULL;
};

