#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent) {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mod = 0.0f;
	//particleSize = 0.5f;
	step = false;

	smileyTex = loadTexture("smiley.png");
	staticTex = loadTexture("noise.png");
}

Renderer::~Renderer(void) {
	glDeleteTextures(1, &smileyTex);
	glDeleteTextures(1, &staticTex);
}

void	Renderer::RenderScene() {
	for (vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i) {
		Render(*(*i));
	}
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	if (o.GetShader() && o.GetMesh()) {
		GLuint program = o.GetShader()->GetProgram();

		glUseProgram(program);

		UpdateShaderMatrices();
		ApplyShaderLight(program);

		glUniform1f(glGetUniformLocation(program, "mod"), mod);
		//glUniform1f(glGetUniformLocation(program, "particleSize"), particleSize);

		glUniform1i(glGetUniformLocation(program, "smileyTex"), 0);
		glUniform1i(glGetUniformLocation(program, "staticTex"), 1);

		UpdateShaderMatrices();

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, o.GetTexture());
		glBindTexture(GL_TEXTURE_2D, smileyTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, staticTex);

		o.Draw();
	}

	for (vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i) {
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) {
	for (vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i) {
		(*i)->Update(msec);
	}

	//increment step values based on msec
	if (step)
	{
		mod += msec * 0.4f;
		if (mod > 2000)
		{
			mod = 0.0f;
		}
	}
}

GLuint Renderer::loadTexture(const char* textIn)
{
	GLuint texture;
	texture = SOIL_load_OGL_texture(textIn, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texture;
}