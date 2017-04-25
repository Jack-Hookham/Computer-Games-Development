#pragma once

#include <GL/glew.h>
#include <string>

#include "Texture.h"
#include "picoPNG.h"
#include "FileManager.h"

//Uses picoPNG to load textures from image files

class ImageManager
{
public:
	static Texture loadTexture(std::string filePath);

private:
	static void log(const std::string text);
};

