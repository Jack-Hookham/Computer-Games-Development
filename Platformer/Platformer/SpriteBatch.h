#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "Vertex.h"
#include "GLTexture.h"

enum BufferType
{
	VERTEX_BUFFER,
	COLOUR_BUFFER,
	TEXTURE_BUFFER,
};

//Type of sorting used for sorting the vector of quads
enum QuadSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

//Quad to hold sprite data
class Quad
{
public:
	//Default constructor
	Quad();

	//Params constructor no angle
	Quad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour);

	//Params constructor with angle
	Quad(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& color, float angle);

	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;

private:
	//Rotate a point about (0, 0) by angle
	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);
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

	void addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour);
	void addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour, float angle);

	void renderBatches();

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
	GLuint mBufferObject = 0;

	//VAO for the sprite
	GLuint mArrayObject = 0;

	//Quads
	std::vector<Quad> mQuads;
	//Quad pointers for sorting
	std::vector<Quad*> mQuadPointers;

	//RenderBatches
	std::vector<RenderBatch> mRenderBatches;

	//SortType
	QuadSortType mSortType;
};

