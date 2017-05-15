#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

#include "Keyboard.h"
#include "Mouse.h"
#include "Controller.h"

//Store information on the user input for this frame and the previous frame
//Currently takes keyboard, mouse and controller input

class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	inline const Keyboard* getKeyboard() const { return mKeyboard; }
	inline const Mouse* getMouse() const { return mMouse; }
	inline const Controller* getController() const { return mController; }

	inline Keyboard* updateKeyboard() const { return mKeyboard; }
	inline Mouse* updateMouse() const { return mMouse; }
	inline Controller* updateController() const { return mController; }

private:
	Keyboard* mKeyboard;
	Mouse* mMouse;
	Controller* mController;
};

