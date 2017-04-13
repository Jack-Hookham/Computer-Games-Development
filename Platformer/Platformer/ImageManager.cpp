#include "ImageManager.h"

GLTexture ImageManager::loadTexture(std::string filePath)
{
	//Initialise texure values to 0;
	GLTexture texture = {};

	//Input for picoPNG decodePNG
	std::vector<unsigned char> input;
	//Output from picoPNG decodePNG
	std::vector<unsigned char> output;

	unsigned long width;
	unsigned long height;

	if (!FileManager::readFile(filePath, input))
	{
		log("Failed to load image file!");
	}

	//Decode the .png format into an array of pixels
	int errorCode = decodePNG(output, width, height, &(input[0]), input.size());
	if (errorCode != 0)
	{
		log("decodePNG error code: " + std::to_string(errorCode));
	}


	glGenTextures(1, &(texture.id));
	//Bind
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(output[0]));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;
	texture.filePath = filePath;

	return texture;
}

void ImageManager::log(const std::string text)
{
	std::cout << "[ImageManager] " << text << std::endl;
}
