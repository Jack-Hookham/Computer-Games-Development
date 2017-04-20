#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

class SpriteSheet
{
public:
	void init(const Texture& texture, const glm::ivec2& dimensions)
	{
		mTexture = texture;
		mDimensions = dimensions;
	}

	inline const Texture getTexture() const { return mTexture; }
	inline const glm::ivec2 getDimensions() const { return mDimensions; }
	const glm::vec4 getTexCoords(const int index) const;

private:
	Texture mTexture;
	glm::ivec2 mDimensions;
};