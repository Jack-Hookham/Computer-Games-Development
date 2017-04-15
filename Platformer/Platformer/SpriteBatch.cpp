#include "SpriteBatch.h"

SpriteBatch::SpriteBatch() : mBufferObject(0), mArrayObject(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

//Initialise the spritebatch
void SpriteBatch::init()
{
	bufferData();
}

//Begin the spritebatch
void SpriteBatch::begin(GlyphSortType sortType)
{
	mSortType = sortType;
	mRenderBatches.clear();
	mGlyphs.clear();
}

//End the spritebatch
void SpriteBatch::end()
{
	sortGlyphs();
	createRenderBatches();
}

//Adds glyph containing information about an indivudual quad used in the SpriteBatch 
//for recTo and texCoord vec4s:
//x = x coord
//y = y coord
//z = width
//w = height
void SpriteBatch::draw(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour)
{
	Glyph* newGlyph = new Glyph;

	newGlyph->texture = texture;
	newGlyph->depth = depth;
	
	newGlyph->topLeft.colour = colour;
	newGlyph->topLeft.setPosition(destQuad.x, destQuad.y + destQuad.w);
	newGlyph->topLeft.setTexCoord(texCoord.x, texCoord.y + texCoord.w);
	
	newGlyph->topRight.colour = colour;
	newGlyph->topRight.setPosition(destQuad.x + destQuad.z, destQuad.y + destQuad.w);
	newGlyph->topRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y + texCoord.w);
	
	newGlyph->bottomLeft.colour = colour;
	newGlyph->bottomLeft.setPosition(destQuad.x, destQuad.y);
	newGlyph->bottomLeft.setTexCoord(texCoord.x, texCoord.y);
	
	newGlyph->bottomRight.colour = colour;
	newGlyph->bottomRight.setPosition(destQuad.x + destQuad.z, destQuad.y);
	newGlyph->bottomRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y);

	mGlyphs.push_back(newGlyph);
}

//Render the spritebatch
void SpriteBatch::renderBatch()
{
	glBindVertexArray(mArrayObject);

	for (int i = 0; i < mRenderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, mRenderBatches[i].offset, mRenderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

//Generate VBO and VAO
void SpriteBatch::bufferData()
{
	if (mArrayObject == 0)
	{
		glGenVertexArrays(1, &mArrayObject);
	}
	glBindVertexArray(mArrayObject);

	if (mBufferObject == 0)
	{
		glGenBuffers(1, &mBufferObject);
	}
	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	glEnableVertexAttribArray(VERTEX_BUFFER);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(COLOUR_BUFFER);
	glVertexAttribPointer(COLOUR_BUFFER, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));

	glEnableVertexAttribArray(TEXTURE_BUFFER);
	glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	//Once we're done with the vertex buffer binding, we can unbind the VAO, ready to reapply later
	glBindVertexArray(0);
}

void SpriteBatch::sortGlyphs()
{
	switch (mSortType)
	{
	case BACK_TO_FRONT:
		std::stable_sort(mGlyphs.begin(), mGlyphs.end(), compareFrontToBack);

	case FRONT_TO_BACK:
		std::stable_sort(mGlyphs.begin(), mGlyphs.end(), compareBackToFront);

	case TEXTURE:
		std::stable_sort(mGlyphs.begin(), mGlyphs.end(), compareTexture);
	}
}

//True if glyph a has a smaller depth than glyph b
bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
{
	return (a->depth < b->depth);
}

//True if glyph has a greater depth than glyph b 
bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
{
	return (a->depth > b->depth);
}

//True if glyph a has a smaller texture id than glyph b
//Sorting by texture means that sprites with the same texture will be drawn one after the other
bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
{
	return (a->texture < b->texture);
}

void SpriteBatch::createRenderBatches()
{
	//Vertices for the glyph
	std::vector<Vertex> vertices;
	//Allocate memory for the vector by resizing it to the size needed
	vertices.resize(mGlyphs.size() * GLYPH_VERTICES);

	if (mGlyphs.empty())
	{
		return;
	}

	//current vertex
	int currentVertex = 0;

	//Add the first batch
	mRenderBatches.emplace_back(0, GLYPH_VERTICES, mGlyphs[0]->texture);
	vertices[currentVertex++] = mGlyphs[0]->topLeft;
	vertices[currentVertex++] = mGlyphs[0]->bottomLeft;
	vertices[currentVertex++] = mGlyphs[0]->bottomRight;
	vertices[currentVertex++] = mGlyphs[0]->bottomRight;
	vertices[currentVertex++] = mGlyphs[0]->topRight;
	vertices[currentVertex++] = mGlyphs[0]->topLeft;

	//Add the rest of the glyphs
	for (int i = 1; i < mGlyphs.size(); i++)
	{
		//if this glyph can't be part of the current batch (different textures)
		if (mGlyphs[i]->texture != mGlyphs[i - 1]->texture)
		{
			//Make a new batch
			mRenderBatches.emplace_back(currentVertex, GLYPH_VERTICES, mGlyphs[0]->texture);
		}
		//if this glyph can be part of the current batch (same textures)
		else
		{
			//increase the number of vertices
			mRenderBatches.back().numVertices += GLYPH_VERTICES;
		}
		vertices[currentVertex++] = mGlyphs[i]->topLeft;
		vertices[currentVertex++] = mGlyphs[i]->bottomLeft;
		vertices[currentVertex++] = mGlyphs[i]->bottomRight;
		vertices[currentVertex++] = mGlyphs[i]->bottomRight;
		vertices[currentVertex++] = mGlyphs[i]->topRight;
		vertices[currentVertex++] = mGlyphs[i]->topLeft;
	}

	//bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	//Orphan and upload the vertice data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	//unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
