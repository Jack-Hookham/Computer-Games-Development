#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "Glyph.h"
#include "Vertex.h"
#include "Sprite.h"
#include "RenderBatch.h"

//Type of sorting used for sorting the vector of glyphs
enum GlyphSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin(GlyphSortType sortType = TEXTURE);
	void end();

	void draw(const glm::vec4& rectTo, const glm::vec4& texCoord, GLuint texture, const Colour& colour, float depth);

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

