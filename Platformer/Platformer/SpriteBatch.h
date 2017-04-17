#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>


#include "Vertex.h"
#include "Sprite.h"
//#include "Glyph.h"
//#include "RenderBatch.h"

//Type of sorting used for sorting the vector of glyphs
enum QuadSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

//Quad to hold sprite data
struct Quad
{
	Quad() {};
	Quad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour) :
		texture(texture), depth(depth)
	{

		topLeft.colour = colour;
		topLeft.setPosition(destQuad.x, destQuad.y + destQuad.w);
		topLeft.setTexCoord(texCoord.x, texCoord.y + texCoord.w);

		topRight.colour = colour;
		topRight.setPosition(destQuad.x + destQuad.z, destQuad.y + destQuad.w);
		topRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y + texCoord.w);

		bottomLeft.colour = colour;
		bottomLeft.setPosition(destQuad.x, destQuad.y);
		bottomLeft.setTexCoord(texCoord.x, texCoord.y);

		bottomRight.colour = colour;
		bottomRight.setPosition(destQuad.x + destQuad.z, destQuad.y);
		bottomRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y);
	}


	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};

//Each render batch is used for a single draw call
struct RenderBatch 
{
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
		numVertices(NumVertices), texture(Texture) {}

	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void bufferData();

	void begin(QuadSortType sortType = TEXTURE);
	void end();

	void addQuad(const glm::vec4& rectTo, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour);

	void renderBatch();

private:
	//2 trianges = 6 vertices
	const unsigned int QUAD_VERTICES = 6;

	void sortQuads();

	//Sorting functions used for std::stable_sort in sortQuads()
	static bool compareFrontToBack(Quad* a, Quad* b);
	static bool compareBackToFront(Quad* a, Quad* b);
	static bool compareTexture(Quad* a, Quad* b);

	void createRenderBatches();

	//VBO for the sprite
	GLuint mBufferObject;

	//VAO for the sprite
	GLuint mArrayObject;

	//Quads
	std::vector<Quad> mQuads;
	//Quad pointers for sorting
	std::vector<Quad*> mQuadPointers;

	std::vector<RenderBatch> mRenderBatches;

	QuadSortType mSortType;
};

