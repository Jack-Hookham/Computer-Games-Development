#include "SpriteSheet.h"

//Calculate the texCoords for the given index
const glm::vec4 SpriteSheet::getTexCoords(const int index) const
{
	int xTile = index % mDimensions.x;
	int yTile = index / mDimensions.x;

	glm::vec4 texCoords;
	texCoords.x = xTile / (float)mDimensions.x;
	texCoords.y = 1.0f - yTile / (float)mDimensions.y;
	texCoords.z = 1.0f / mDimensions.x;
	texCoords.w = 1.0f / mDimensions.y;

	return texCoords;
}
