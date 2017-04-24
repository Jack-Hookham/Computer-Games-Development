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

	inline const int getWidth() const { return mWindowWidth; }
	inline const int getHeight() const { return mWindowHeight; }

	inline void setWidth(const int width) { mWindowWidth = width; }
	inline void setHeight(const int height) { mWindowHeight = height; }

private:
	//Log for Window
	void log(const std::string text);

	SDL_Window* mSDLwindow;
	int mWindowWidth;
	int mWindowHeight;
};

