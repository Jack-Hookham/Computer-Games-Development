#include "InputManager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::pressKey(unsigned int keyID)
{
	mKeyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID)
{
	mKeyMap[keyID] = false;
}

//True if key is held down
bool InputManager::isKeyDown(unsigned int keyID)
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
bool InputManager::isKeyPressed(unsigned int keyID)
{
	if (isKeyDown(keyID) && !wasKeyDown(keyID)) 
	{
		return true;
	}
	return false;
}

//True if key was down in the previous map but isn't down anymore
bool InputManager::isKeyReleased(unsigned int keyID)
{
	if (!isKeyDown(keyID) && wasKeyDown(keyID))
	{
		return true;
	}
	return false;
}

void InputManager::setMouseCoords(float x, float y)
{
	mMouseCoords.x = x;
	mMouseCoords.y = y;

	//cout << mMouseCoords << endl;
}

glm::vec2 InputManager::getMouseCoords()
{
	return mMouseCoords;
}

//Copy current keymap to previous keymap
void InputManager::update()
{
	for (std::pair<unsigned int, bool> i : mKeyMap)
	{
		mPrevKeyMap[i.first] = i.second;
	}
}

bool InputManager::wasKeyDown(unsigned int keyID)
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

void InputManager::log(const std::string text)
{
	std::cout << "[InputManager] " << text << std::endl;
}
