#include "Tutorials.h"
#include "../../Graphics/nclgl/Window.h"
#include "Renderer.h"

Tutorials::Tutorials()
{
}


Tutorials::~Tutorials()
{
}

void Tutorials::tutorialOne()
{
	Window w("My First OpenGL 3 Triangle", 800, 600, false);

	if (!w.HasInitialised())
	{
		return;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised())
	{
		return;
	}
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE))
	{
		renderer.RenderScene();
	}
}

void Tutorials::tutorialTwo()
{
}

void Tutorials::tutorialThree()
{
}

void Tutorials::tutorialFour()
{
}

void Tutorials::tutorialFive()
{
}

void Tutorials::tutorialSix()
{
}

void Tutorials::tutorialSeven()
{
}

void Tutorials::tutorialEight()
{
}

void Tutorials::tutorialNine()
{
}
