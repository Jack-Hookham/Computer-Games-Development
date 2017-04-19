#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class TileSheet
{
public:
	void init(const Texture& texture, const glm::ivec3& tileDimensions);

	glm::vec4 getTexCoords(int index);

	Texture texture;
	glm::ivec3 dimensions;
};