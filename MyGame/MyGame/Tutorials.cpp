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
	Window w = Window("Triangle", 800, 600, false);
	if (!w.HasInitialised())
	{
		return;
	}

	Renderer r(w);
	if (!r.HasInitialised())
	{
		return;
	}

	Mesh*	m = Mesh::GenerateTriangle();
	Shader* s = new Shader("../Shaders/basicVert.glsl", "../Shaders/basicFrag.glsl");
	//Shader* s = new Shader("../Shaders/basicVertex.glsl", "../Shaders/colourFragment.glsl");
	//Shader* s = new Shader("basicVert.glsl", "basicFrag.glsl", "pointCubeGeom.glsl");

	if (s->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}

	RenderObject* o = new RenderObject(m, s);
	r.AddRenderObject(*o);

	r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 10)));

	while (w.UpdateWindow())
	{
		float msec = w.GetTimer()->GetTimedMS();

		r.UpdateScene(msec);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}

	delete m;
	delete s;
	delete o;
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
