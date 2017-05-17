#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//Saves the top 5 highscores
class Highscores
{
public:
	Highscores();
	~Highscores();

	void writeScores();
	//if the score is high enough add it and return its index
	const int addScore(const int score);

	const int getScore(int index) const { return mScores[index]; }
	const int getNumScores() const { return NUM_SCORES; }

private:
	void log(const std::string text);

	std::string mPath = "../res/highscores/highscores.txt";

	const int NUM_SCORES = 5;
	int mScores[5] = { 0, 0, 0, 0, 0 };
};

