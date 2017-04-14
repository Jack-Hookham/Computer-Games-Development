#pragma once

#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>

class Window
{
public:
	Window();
	~Window();

	//Create and setup the window
	bool createWindow(std::string name, int screenWidth, int screenHeight, Uint32 flags);

	void swapWindow();

	int getWidth();
	int getHeight();

	void setWidth(int width);
	void setHeight(int height);

private:
	//Log for Window
	void log(const std::string text);

	SDL_Window* mSDLwindow;
	int mWindowWidth;
	int mWindowHeight;
};

