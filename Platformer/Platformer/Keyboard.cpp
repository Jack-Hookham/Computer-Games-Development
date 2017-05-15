#include "Keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::pressKey(unsigned int keyID)
{
	mKeyMap[keyID] = true;
}

void Keyboard::releaseKey(unsigned int keyID)
{
	mKeyMap[keyID] = false;
}

//True if key is held down
const bool Keyboard::isKeyDown(unsigned int keyID) const
{
	auto key = mKeyMap.find(keyID);
	//if key was found
	if (key != mKeyMap.end())
	{
		//return the boolean for that key
		return key->second;
	}
	return false;
}

//True if key is pressed but not held
const bool Keyboard::isKeyPressed(unsigned int keyID) const
{
	if (isKeyDown(keyID) && !wasKeyDown(keyID))
	{
		return true;
	}
	return false;
}

//True if key was down in the previous map but isn't down anymore
const bool Keyboard::isKeyReleased(unsigned int keyID) const
{
	if (!isKeyDown(keyID) && wasKeyDown(keyID))
	{
		return true;
	}
	return false;
}

//Copy current keymap to previous keymap
void Keyboard::update()
{
	for each (std::pair<unsigned int, bool> i in mKeyMap)
	{
		mPrevKeyMap[i.first] = i.second;
	}
}

//Was the key down last frame?
const bool Keyboard::wasKeyDown(unsigned int keyID) const
{
	auto key = mPrevKeyMap.find(keyID);
	if (key != mPrevKeyMap.end())
	{
		//Key found
		return key->second;
	}
	else
	{
		//Key not found
		return false;
	}
}

void Keyboard::log(const std::string text)
{
	std::cout << "[Keyboard] " << text << std::endl;
}
