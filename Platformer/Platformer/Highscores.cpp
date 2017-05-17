#include "Highscores.h"

Highscores::Highscores()
{
	std::ifstream file;
	file.open(mPath);
	if (file.fail())
	{
		log("Failed to load high scores file " + mPath);
	}

	//Store the current line
	std::string score;

	//Search the file
	while (!file.eof())
	{
		int index = 0;
		while (std::getline(file, score))
		{
			mScores[index] = stoi(score);
			index++;
		}
	}

	file.close();
}


Highscores::~Highscores()
{
}

void Highscores::writeScores()
{
	std::ofstream file(mPath);
	if (file.is_open())
	{
		for (int i = 0; i < NUM_SCORES; i++)
		{
			file << mScores[i] << std::endl;
		}
	}
}

void Highscores::addScore(int score)
{
	for (int i = 0; i < NUM_SCORES; i++)
	{
		if (score > mScores[i])
		{
			//Move all scores after i down 1
			for (int j = NUM_SCORES - 1; j > i; j--)
			{
				mScores[j] = mScores[j - 1];
			}

			//Update the score
			mScores[i] = score;
			break;
		}
	}
}

void Highscores::log(const std::string text)
{
	std::cout << "[Highscores] " << text << std::endl;
}