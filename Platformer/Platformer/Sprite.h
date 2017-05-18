#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Vertex.h"

//Sprite class to hold sprite data for sprites used in SpriteBatch

class Sprite
{
public:
	//Default constructor
	Sprite();

	//Params constructor no angle
	Sprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture,
		float depth, const Colour& colour);

	//Params constructor with angle
	Sprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& uvRect, GLuint Texture,
		float Depth, const Colour& color, float angle);

	const GLuint getTexure() const { return texture; }
	const float getDepth() const { return depth; }

	const Vertex getTL() const { return topLeft; }
	const Vertex getTR() const { return topRight; }
	const Vertex getBL() const { return bottomLeft; }
	const Vertex getBR() const { return bottomRight; }

private:
	//Rotate a point about (0, 0) by angle
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);

	GLuint texture;
	float depth;

	//Vertices
	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};