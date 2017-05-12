#pragma once

#include <GL/glew.h>
#include <string>

//Struct containing texture information

struct Texture
{
	std::string filePath;
	GLuint id;
	int width;
	int height;
};