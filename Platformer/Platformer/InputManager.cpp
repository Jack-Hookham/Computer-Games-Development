#include "InputManager.h"

InputManager::InputManager()
{
	mKeyboard = new Keyboard;
	mMouse = new Mouse;
	mController = new Controller;
}

InputManager::~InputManager()
{
	delete mKeyboard;
	delete mMouse;
	delete mController;
}

//Copies all input maps to previous input maps
void InputManager::update()
{
	mKeyboard->update();
	mMouse->update();
	mController->update();
}
