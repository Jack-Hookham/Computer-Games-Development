#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

class Controller
{
public:
	Controller();
	~Controller();

	//Button press and release
	void pressButton(unsigned int buttonID);
	void releaseButton(unsigned int buttonID);

	//Button status bools
	const bool isButtonDown(unsigned int buttonID) const;
	const bool isButtonPressed(unsigned int buttonID) const;
	const bool isButtonReleased(unsigned int buttonID) const;

	void update();

	//Controller stick
	const int getLeftStickDirection() const { return mLeftStickDirection; }
	void setLeftStickDirection(const int direction) { mLeftStickDirection = direction; }

private:
	//Was key down last frame
	const bool wasButtonDown(unsigned int keyID) const;

	//Log for input manager
	void log(const std::string text);

	//Two button maps for storing key presses
	//Previous key map stores key presses from the previous frame for
	//differentiating between a key being pressed and a key being held
	std::unordered_map<unsigned int, bool> mButtonMap;
	std::unordered_map<unsigned int, bool> mPrevButtonMap;

	//Controller left stick direction
	//left = -1
	//middle = 0
	//right = 1
	int mLeftStickDirection = 0;
};

