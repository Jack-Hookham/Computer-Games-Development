#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <string>

#include "GLTexture.h"
#include "Vertex.h"
#include "ResourceManager.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath);

	void draw();

private:
	float mX;
	float mY;
	float mWidth;
	float mHeight;
	GLuint vboID;
	GLTexture mTexture;
};

