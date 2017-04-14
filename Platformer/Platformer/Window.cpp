#include "Window.h"

Window::Window()
{
}

Window::~Window()
{
}

bool Window::createWindow(std::string name, int screenWidth, int screenHeight, Uint32 flags)
{
	//Init flag
	bool success = true;

	mWindowWidth = screenWidth;
	mWindowHeight = screenHeight;

	//Uint32 flags = SDL_WINDOW_OPENGL;

	//if (currentFlags & INVISIBLE) 
	//{
	//	flags |= SDL_WINDOW_HIDDEN;
	//}
	//if (currentFlags & FULLSCREEN) 
	//{
	//	flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	//}
	//if (currentFlags & BORDERLESS) 
	//{
	//	flags |= SDL_WINDOW_BORDERLESS;
	//}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Set texture filtering to linear
	//if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	//{
	//	log("Warning: Linear texture filtering not enabled!");
	//}

	//Create window and error check
	mSDLwindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, flags);
	if (mSDLwindow == NULL)
	{
		const std::string text = "Window could not be created! SDL Error: " + std::string(SDL_GetError());
		log(text);
		success = false;
	}
	else
	{
		//Create renderer for window
		//mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		//if (mRenderer == NULL)
		//{
		//	const std::string text = "Renderer could not be created! SDL Error: " + std::string(SDL_GetError());
		//	log(text);
		//	success = false;
		//}

		//Create context and error check
		SDL_GLContext glContext = SDL_GL_CreateContext(mSDLwindow);
		if (glContext == NULL)
		{
			const std::string text = "OpenGL context could not be created! SDL Error: " + std::string(SDL_GetError());
			log(text);
			success = false;
		}
		else
		{
			//Initialise GLEW and error check
			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (glewError != GLEW_OK)
			{
				log("Error initializing GLEW!");
			}

			//Print the OpenGL version to the console
			log("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));

			//Set the background colour
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

			//Use Vsync
			//if (SDL_GL_SetSwapInterval(1) < 0)
			//{
			//	log("Warning: Unable to set VSync! SDL Error: " + std::string(SDL_GetError()));
			//}
		}
	}

	return success;
}

void Window::swapWindow()
{
	//Swap buffer and draw everything
	SDL_GL_SwapWindow(mSDLwindow);
}

int Window::getWidth()
{
	return mWindowWidth;
}

int Window::getHeight()
{
	return mWindowHeight;
}

void Window::setWidth(int width)
{
	mWindowWidth = width;
}

void Window::setHeight(int height)
{
	mWindowHeight = height;
}

void Window::log(const std::string text)
{
	std::cout << "[Window] " << text << std::endl;
}
