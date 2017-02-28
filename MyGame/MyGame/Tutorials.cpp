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
	//Window w = Window("Triangle", 800, 600, false);
	//if (!w.HasInitialised())
	//{
	//	return;
	//}

	//Renderer r(w);
	//if (!r.HasInitialised())
	//{
	//	return;
	//}

	//Mesh*	m = Mesh::GenerateTriangle();
	//Shader* s = new Shader("../Shaders/basicVert.glsl", "../Shaders/basicFrag.glsl");
	////Shader* s = new Shader("basicVert.glsl", "basicFrag.glsl", "pointCubeGeom.glsl");

	//if (s->UsingDefaultShader()) {
	//	cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
	//	cout << "Press any key to continue." << endl;
	//	std::cin.get();
	//}

	//float pitch = -30.0f;
	//float yaw = 0.0f;
	//float scale = 100.0f;
	//float rotation = 0.0f;
	//Vector3 position(0.0f, 0.0f, 0.0f);

	//RenderObject* o = new RenderObject(m, s);
	////o->SetModelMatrix(Matrix4::Rotation(45.0f, Vector3(1, 1, 0)));
	//r.AddRenderObject(*o);

	////r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 10)));

	//while (w.UpdateWindow())
	//{
	//	float msec = w.GetTimer()->GetTimedMS();
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
	//	//	r.SwitchToOrthographic();
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
	//	//	r.SwitchToPerspective();
	//	//
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS)) ++scale;
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) --scale;
	//	//
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) ++rotation;
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) --rotation;
	//	//
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
	//	//	position.y -= 1.0f;
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
	//	//	position.y += 1.0f;
	//	//
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
	//	//	position.x -= 1.0f;
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
	//	//	position.x += 1.0f;
	//	//
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
	//	//	position.z -= 1.0f;
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_P))
	//	//	position.z += 1.0f;	//	//movement controls
	//	//for each(RenderObject* o in plain) {
	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
	//	//	position += Matrix4::Rotation(yaw, Vector3(0.0f, 0.05f, 0.0f)) *
	//	//		Vector3(0.0f, 0.0f, -0.05f) * msec;
	//	//	//o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0.2, 0, 0)));
	//	//}

	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
	//	//	position -= Matrix4::Rotation(yaw, Vector3(0.0f, 0.05f, 0.0f)) *
	//	//		Vector3(0.0f, 0.0f, -0.05f)* msec;
	//	//	//o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(-0.2, 0, 0)));
	//	//}

	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
	//	//	position += Matrix4::Rotation(yaw, Vector3(0.0f, 0.05f, 0.0f)) *
	//	//		Vector3(-0.1f, 0.0f, 0.0f) * msec;
	//	//	//o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0, 0, 0.2)));
	//	//}

	//	//if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
	//	//	position -= Matrix4::Rotation(yaw, Vector3(0.0f, 0.05f, 0.0f)) *
	//	//		Vector3(-0.1f, 0.0f, 0.0f) * msec;
	//	//	//o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0, 0, -0.2)));
	//	//}	//	////if (Window::GetMouse()->ButtonHeld) {
	//	//	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	//	//	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	//	//	pitch = min(pitch, 90.0f);
	//	//	pitch = max(pitch, -90.0f);

	//	//	if (yaw < 0) {
	//	//		yaw += 360.0f;
	//	//	}
	//	//	if (yaw > 360.0f) {
	//	//		yaw -= 360.0f;
	//	//	}	//	//}
	//	/*r.SetViewMatrix(Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
	//		Matrix4::Rotation(-yaw, Vector3(0, 1, 0))*
	//		Matrix4::Translation(-position));*/

	//	r.UpdateScene(msec);
	//	r.ClearBuffers();
	//	r.RenderScene();
	//	r.SwapBuffers();
	//}

	//delete m;
	//delete s;
	//delete o;

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

	Vector3 position(0.0f, 0.0f, 0.0f);

	r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, 10)));

	while (w.UpdateWindow())
	{
		float msec = w.GetTimer()->GetTimedMS();

			//movement controls
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
				//position.y += 1.0f * msec;
				o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0, 0.01, 0)));
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
				//position.y -= 1.0f * msec;
				o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0, -0.01, 0)));
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
				//position.x += 1.0f * msec;
				o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(0.01, 0, 0)));
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
				//position.x -= 1.0f * msec;
				o->SetModelMatrix(o->GetModelMatrix() * Matrix4::Translation(Vector3(-0.01, 0, 0)));
			}

			//r.SetViewMatrix(Matrix4::Translation(position));

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
	Window w = Window("Triangles", 800, 600, false);
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
	//Shader* s = new Shader("basicVert.glsl", "basicFrag.glsl", "pointCubeGeom.glsl");

	if (s->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}

	float scale = 100.0f;
	float rotation = 0.0f;
	Vector3 position(-100.0f, 0.0f, -1500.0f);


	for (int i = 0; i < 3; i++)
	{
		RenderObject* o = new RenderObject(m, s);
		o->SetModelMatrix(	Matrix4::Translation(position) * 
							Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * 
							Matrix4::Scale(Vector3(scale, scale, scale)));
		r.AddRenderObject(*o);
		position.z += 500.0f;
	}

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
	//delete o;
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
