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
	//if the score is high enough add it
	void addScore(int score);
	const int getScore(int index) { return mScores[index]; }

private:
	void log(const std::string text);

	std::string mPath = "../res/highscores/highscores.txt";

	const int NUM_SCORES = 5;
	int mScores[5] = { 0, 0, 0, 0, 0 };
};

