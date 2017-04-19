#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class SpriteSheet
{
public:
	void init(const Texture& texture, const glm::ivec2& dimensions)
	{
		this->texture = texture;
		this->dimensions = dimensions;
	}

	glm::vec4 getTexCoords(int index)
	{
		int xTile = index % dimensions.x;
		int yTile = index / dimensions.x;

		std::cout << "x: " << xTile << "y: " << yTile << std::endl;

		glm::vec4 texCoords;
		texCoords.x = xTile / (float)dimensions.x;
		texCoords.y = 1.0f - (yTile / (float)dimensions.y);
		texCoords.z = 1.0f / dimensions.x;
		texCoords.w = 1.0f / dimensions.y;

		return texCoords;
	}

	Texture texture;
	glm::ivec2 dimensions;
};