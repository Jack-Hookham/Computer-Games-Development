#pragma once

#include <unordered_map>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);

	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);
	bool isKeyReleased(unsigned int keyID);

	void update();

private:
	bool wasKeyDown(unsigned int keyID);

	//Two key maps for storing key presses
	//Previous key map stores key presses from the previous frame for
	//differentiating between a key being pressed and a key being held
	std::unordered_map<unsigned int, bool> mKeyMap;
	std::unordered_map<unsigned int, bool> mPrevKeyMap;
};

