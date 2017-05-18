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

	const int getWidth() const { return mWindowWidth; }
	const int getHeight() const { return mWindowHeight; }
	SDL_Window* getSDLWindow() const { return mSDLwindow; }

	void setWidth(const int width) { mWindowWidth = width; }
	void setHeight(const int height) { mWindowHeight = height; }

private:
	//Log for Window
	void log(const std::string text);

	SDL_Window* mSDLwindow;
	int mWindowWidth;
	int mWindowHeight;
};

