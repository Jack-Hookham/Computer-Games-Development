#pragma once

#include <GL/glew.h>
#include <string>

struct Texture
{
	std::string filePath = "";
	GLuint id;
	int width;
	int height;
};