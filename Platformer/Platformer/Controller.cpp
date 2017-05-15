#include "Controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::pressButton(unsigned int buttonID)
{
	mButtonMap[buttonID] = true;
}

void Controller::releaseButton(unsigned int buttonID)
{
	mButtonMap[buttonID] = false;
}

//True if button is held down
const bool Controller::isButtonDown(unsigned int buttonID) const
{
	auto button = mButtonMap.find(buttonID);
	//if button was found
	if (button != mButtonMap.end())
	{
		//return the boolean for that button
		return button->second;
	}
	return false;
}

//True if button is pressed but not held
const bool Controller::isButtonPressed(unsigned int buttonID) const
{
	if (isButtonDown(buttonID) && !wasButtonDown(buttonID))
	{
		return true;
	}
	return false;
}

//True if button was down in the previous map but isn't down anymore
const bool Controller::isButtonReleased(unsigned int buttonID) const
{
	if (!isButtonDown(buttonID) && wasButtonDown(buttonID))
	{
		return true;
	}
	return false;
}

//Copy current buttonmap to previous buttonmap
void Controller::update()
{
	for each (std::pair<unsigned int, bool> i in mButtonMap)
	{
		mPrevButtonMap[i.first] = i.second;
	}
}

//Was the button down last frame?
const bool Controller::wasButtonDown(unsigned int buttonID) const
{
	auto button = mPrevButtonMap.find(buttonID);
	if (button != mPrevButtonMap.end())
	{
		//Button found
		return button->second;
	}
	else
	{
		//Button not found
		return false;
	}
}

void Controller::log(const std::string text)
{
	std::cout << "[Controller] " << text << std::endl;
}