#include "SoundEffect.h"

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{
}

//Play sound n times
bool SoundEffect::play(int n)
{
	bool success = true;

	//Attempt to play on first free unreserved channel
	if (Mix_PlayChannel(-1, mChunk, n) == -1)
	{
		//if no channels are free attempt to play on channel 0
		if (Mix_PlayChannel(0, mChunk, n) == -1)
		{
			return success = false;
		}
	}

	return success;
}
