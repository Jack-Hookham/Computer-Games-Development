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

bool InputManager::isKeyPressed(unsigned int keyID)
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
