#include "Sprite.h"

Sprite::Sprite()
{
}

//Params constructor no angle
Sprite::Sprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture,
	float depth, const Colour& colour)
{
	this->texture = texture;
	this->depth = depth;

	//Set the params for each vertex
	topLeft.setColour(colour);
	topLeft.setPosition(position.x, position.y + dimensions.y);
	topLeft.setTexCoord(texCoord.x, texCoord.y + texCoord.w);

	topRight.setColour(colour);
	topRight.setPosition(position.x + dimensions.x, position.y + dimensions.y);
	topRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y + texCoord.w);

	bottomLeft.setColour(colour);
	bottomLeft.setPosition(position.x, position.y);
	bottomLeft.setTexCoord(texCoord.x, texCoord.y);

	bottomRight.setColour(colour);
	bottomRight.setPosition(position.x + dimensions.x, position.y);
	bottomRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y);
}

//Params constructor with angle
Sprite::Sprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture,
	float depth, const Colour& colour, float angle)
{
	this->texture = texture;
	this->depth = depth;

	glm::vec2 halfDimensions(dimensions.x / 2.0f, dimensions.y / 2.0f);

	//Get points centered at origin
	glm::vec2 tl(-halfDimensions.x, halfDimensions.y);
	glm::vec2 bl(-halfDimensions.x, -halfDimensions.y);
	glm::vec2 br(halfDimensions.x, -halfDimensions.y);
	glm::vec2 tr(halfDimensions.x, halfDimensions.y);

	//Rotate the points
	tl = rotatePoint(tl, angle) + halfDimensions;
	bl = rotatePoint(bl, angle) + halfDimensions;
	br = rotatePoint(br, angle) + halfDimensions;
	tr = rotatePoint(tr, angle) + halfDimensions;

	//Set the params for each vertex
	topLeft.setColour(colour);
	topLeft.setPosition(position.x + tl.x, position.y + tl.y);
	topLeft.setTexCoord(texCoord.x, texCoord.y + texCoord.w);

	topRight.setColour(colour);
	topRight.setPosition(position.x + tr.x, position.y + tr.y);
	topRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y + texCoord.w);

	bottomLeft.setColour(colour);
	bottomLeft.setPosition(position.x + bl.x, position.y + bl.y);
	bottomLeft.setTexCoord(texCoord.x, texCoord.y);

	bottomRight.setColour(colour);
	bottomRight.setPosition(position.x + br.x, position.y + br.y);
	bottomRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y);
}

glm::vec2 Sprite::rotatePoint(const glm::vec2& pos, float angle)
{
	glm::vec2 newVec2;

	float s = sin(angle);
	float c = cos(angle);

	newVec2.x = pos.x * c - pos.y * s;
	newVec2.y = pos.x * s + pos.y * c;

	return newVec2;
}