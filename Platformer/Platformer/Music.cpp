#include "Music.h"

Music::Music()
{
}


Music::~Music()
{
}

//Play music n times
void Music::play(int n)
{
	Mix_PlayMusic(mMusic, n);
}

void Music::stop()
{
	Mix_HaltMusic();
}

void Music::resume()
{
	Mix_ResumeMusic();
}

void Music::pause()
{
	Mix_PauseMusic();
}
