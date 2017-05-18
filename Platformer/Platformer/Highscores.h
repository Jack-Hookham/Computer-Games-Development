#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

//Saves the top 10 highscores
class Highscores
{
public:
	Highscores();
	~Highscores();

	//Write scores to file
	void writeScores();

	//if the score is high enough add it and return its index
	const int addScore(const int score);

	const int getScore(int index) const { return mScores[index]; }
	const int getNumScores() const { return NUM_SCORES; }

private:
	void log(const std::string text);

	std::string mPath = "../res/highscores/highscores.txt";

	const int NUM_SCORES = 10;
	int mScores[10];
};

