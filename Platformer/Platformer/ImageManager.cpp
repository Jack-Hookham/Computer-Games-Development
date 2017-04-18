#include "ImageManager.h"

Texture ImageManager::loadTexture(std::string filePath)
{
	//Initialise texure values to 0;
	Texture texture = {};

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

	//Generate texture names and store in texture.id
	glGenTextures(1, &(texture.id));
	//Bind texture to target GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, texture.id);

	//Specify a two-dimensional texture image
	//params(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, 
	//GLenum format, GLenum type, const GLvoid * data)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(output[0]));

	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Generate mipmaps for GL_TEXTURE_2D
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture from target GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, 0);

	//Set texture params
	texture.width = width;
	texture.height = height;
	texture.filePath = filePath;

	return texture;
}

//Log function for ImageManager
void ImageManager::log(const std::string text)
{
	std::cout << "[ImageManager] " << text << std::endl;
}
