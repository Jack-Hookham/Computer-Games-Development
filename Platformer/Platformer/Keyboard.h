#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	//Key press and release
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	//Key status bools
	const bool isKeyDown(unsigned int keyID) const;
	const bool isKeyPressed(unsigned int keyID) const;
	const bool isKeyReleased(unsigned int keyID) const;

	void update();

private:
	//Was key down last frame
	const bool wasKeyDown(unsigned int keyID) const;

	//Log for input manager
	void log(const std::string text);

	//Two key maps for storing key presses
	//Previous key map stores key presses from the previous frame for
	//differentiating between a key being pressed and a key being held
	std::unordered_map<unsigned int, bool> mKeyMap;
	std::unordered_map<unsigned int, bool> mPrevKeyMap;
};

