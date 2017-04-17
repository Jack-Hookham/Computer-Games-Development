#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <string>

#include "GLTexture.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include "SpriteBatch.h"

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height, std::string texturePath);

	void draw();

private:
	//Sprite dimensions
	float mX;
	float mY;
	float mWidth;
	float mHeight;

	//VBO for the sprite
	GLuint mBufferObject;

	//Texture for the sprite
	GLTexture mTexture;
};

