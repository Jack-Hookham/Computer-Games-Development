#pragma once
#include "../../Graphics/nclgl/OGLRenderer.h"
#include "RenderObject.h"

#include <vector>

#define PI 3.1415926535897

using std::vector;

struct Light {
	Vector3 position;
	float radius;
	Vector3 colour;
};

class Renderer : public OGLRenderer {
public:
	Renderer(Window &parent);
	~Renderer(void);

	virtual void	RenderScene();
	virtual void	Render(const RenderObject &o);
	virtual void	UpdateScene(float msec);

	void	AddRenderObject(RenderObject &r) {
		renderObjects.push_back(&r);
	}

	void SetShaderLight(Vector3 position, Vector3 colour, float radius) {
		currentLight.position = position;
		currentLight.colour = colour;
		currentLight.radius = radius;
	}

	GLuint loadTexture(const char* textIn);

	float mod;
	//float particleSize;
	bool step;

protected:

	GLuint(smileyTex);
	GLuint(staticTex);
	vector<RenderObject*> renderObjects;



	void ApplyShaderLight(GLuint program) {
		glUniform3fv(glGetUniformLocation(program, "lightColour"),
			1, (float*)&currentLight.colour);
		glUniform3fv(glGetUniformLocation(program, "lightPos"),
			1, (float*)&currentLight.position);
		glUniform1f(glGetUniformLocation(program, "lightRadius"),
			currentLight.radius);
	}

	Light currentLight;
};