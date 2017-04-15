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
enum GlyphSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

struct Glyph
{
	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
};

// Each render batch is used for a single draw call
class RenderBatch {
public:
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
		numVertices(NumVertices), texture(Texture) {
	}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin(GlyphSortType sortType = TEXTURE);
	void end();

	void draw(const glm::vec4& rectTo, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour);

	void renderBatch();

private:
	//glyphs are quads = 2 trianges = 6 vertices
	const unsigned int GLYPH_VERTICES = 6;

	void bufferData();

	void sortGlyphs();

	//Sorting functions used for std::stable_sort in sortGlyphs()
	static bool compareFrontToBack(Glyph* a, Glyph* b);
	static bool compareBackToFront(Glyph* a, Glyph* b);
	static bool compareTexture(Glyph* a, Glyph* b);

	void createRenderBatches();

	//VBO for the sprite
	GLuint mBufferObject;

	//VAO for the sprite
	GLuint mArrayObject;

	std::vector<Glyph*> mGlyphs;
	std::vector<RenderBatch> mRenderBatches;

	GlyphSortType mSortType;
};

