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

	const Keyboard* getKeyboard() const { return mKeyboard; }
	const Mouse* getMouse() const { return mMouse; }
	const Controller* getController() const { return mController; }

	Keyboard* updateKeyboard() const { return mKeyboard; }
	Mouse* updateMouse() const { return mMouse; }
	Controller* updateController() const { return mController; }

private:
	Keyboard* mKeyboard;
	Mouse* mMouse;
	Controller* mController;
};

