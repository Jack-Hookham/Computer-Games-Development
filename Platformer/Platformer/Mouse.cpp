#include "Mouse.h"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

void Mouse::pressButton(unsigned int keyID)
{
	mButtonMap[keyID] = true;
}

void Mouse::releaseButton(unsigned int keyID)
{
	mButtonMap[keyID] = false;
}

//True if key is held down
const bool Mouse::isButtonDown(unsigned int keyID) const
{
	auto key = mButtonMap.find(keyID);
	//if key was found
	if (key != mButtonMap.end())
	{
		//return the boolean for that key
		return key->second;
	}
	return false;
}

//True if key is pressed but not held
const bool Mouse::isButtonPressed(unsigned int keyID) const
{
	if (isButtonDown(keyID) && !wasButtonDown(keyID))
	{
		return true;
	}
	return false;
}

//True if key was down in the previous map but isn't down anymore
const bool Mouse::isButtonReleased(unsigned int keyID) const
{
	if (!isButtonDown(keyID) && wasButtonDown(keyID))
	{
		return true;
	}
	return false;
}

//Copy current keymap to previous keymap
void Mouse::update()
{
	for each (std::pair<unsigned int, bool> i in mButtonMap)
	{
		mPrevButtonMap[i.first] = i.second;
	}
}

//Was the key down last frame?
const bool Mouse::wasButtonDown(unsigned int keyID) const
{
	auto key = mPrevButtonMap.find(keyID);
	if (key != mPrevButtonMap.end())
	{
		//Button found
		return key->second;
	}
	else
	{
		//Button not found
		return false;
	}
}

void Mouse::log(const std::string text)
{
	std::cout << "[Mouse] " << text << std::endl;
}
