#include "SpriteSheet.h"

void SpriteSheet::init(const Texture & texture, const glm::ivec2 & dimensions)
{
	mTexture = texture;
	mDimensions = dimensions;
}

//Calculate the texCoords for the given index
const glm::vec4 SpriteSheet::getTexCoords(const int index) const
{
	//Cycle through the current row
	int xTile = index % mDimensions.x;
	//Move onto the next row when the end of the row is reached
	int yTile = index / mDimensions.x;

	glm::vec4 texCoords;
	texCoords.x = xTile / (float)mDimensions.x;
	texCoords.y = 1.0f - yTile / (float)mDimensions.y;
	texCoords.z = 1.0f / mDimensions.x;
	texCoords.w = 1.0f / mDimensions.y;

	return texCoords;
}
