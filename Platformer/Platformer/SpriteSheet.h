#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

//Holds individual spritesheet information

class SpriteSheet
{
public:
	void init(const Texture& texture, const glm::ivec2& dimensions);

	inline const Texture getTexture() const { return mTexture; }
	inline const glm::ivec2 getDimensions() const { return mDimensions; }
	
	//Calculate the texCoords depending on the current sprite index
	const glm::vec4 getTexCoords(const int index) const;

private:
	Texture mTexture;
	glm::ivec2 mDimensions;
};