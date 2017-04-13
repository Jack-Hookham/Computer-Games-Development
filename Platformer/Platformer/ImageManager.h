#pragma once

#include <string>

#include "GLTexture.h"
#include "picoPNG.h"
#include "FileManager.h"

class ImageManager
{
public:
	static GLTexture loadTexture(std::string filePath);

private:
	static void log(const std::string text);
};

