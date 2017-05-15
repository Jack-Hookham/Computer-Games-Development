#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

class Mouse
{
public:
	Mouse();
	~Mouse();

	//Button press and release
	void pressButton(unsigned int buttonID);
	void releaseButton(unsigned int buttonID);

	//Button status bools
	const bool isButtonDown(unsigned int buttonID) const;
	const bool isButtonPressed(unsigned int buttonID) const;
	const bool isButtonReleased(unsigned int buttonID) const;

	//Mouse
	inline void setMouseCoords(const float x, const float y)
	{
		mMouseCoords.x = x;
		mMouseCoords.y = y;
	}

	inline const glm::vec2 Mouse::getMouseCoords() const { return mMouseCoords; }

	void update();

private:
	//Was button down last frame
	const bool wasButtonDown(unsigned int buttonID) const;

	//Log for input manager
	void log(const std::string text);

	//Two button maps for storing button presses
	//Previous button map stores button presses from the previous frame for
	//differentiating between a button being pressed and a button being held
	std::unordered_map<unsigned int, bool> mButtonMap;
	std::unordered_map<unsigned int, bool> mPrevButtonMap;

	glm::vec2 mMouseCoords;
};

