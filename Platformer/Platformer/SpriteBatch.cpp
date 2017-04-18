#include "SpriteBatch.h"

#include <algorithm>

Quad::Quad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour)
{
	this->texture = texture;
	this->depth = depth;

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

Quad::Quad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour, float angle)
{
	this->texture = texture;
	this->depth = depth;

	glm::vec2 halfDims(destQuad.z / 2.0f, destQuad.w / 2.0f);

	// Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	// Rotate the points
	tl = rotatePoint(tl, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	topLeft.colour = colour;
	topLeft.setPosition(destQuad.x + tl.x, destQuad.y + tl.y);
	topLeft.setTexCoord(texCoord.x, texCoord.y + texCoord.w);

	bottomLeft.colour = colour;
	bottomLeft.setPosition(destQuad.x + bl.x, destQuad.y + bl.y);
	bottomLeft.setTexCoord(texCoord.x, texCoord.y);

	bottomRight.colour = colour;
	bottomRight.setPosition(destQuad.x + br.x, destQuad.y + br.y);
	bottomRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y);

	topRight.colour = colour;
	topRight.setPosition(destQuad.x + tr.x, destQuad.y + tr.y);
	topRight.setTexCoord(texCoord.x + texCoord.z, texCoord.y + texCoord.w);
}

glm::vec2 Quad::rotatePoint(const glm::vec2& pos, float angle) 
{
	glm::vec2 newVec2;
	newVec2.x = pos.x * cos(angle) - pos.y * sin(angle);
	newVec2.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newVec2;
}

SpriteBatch::SpriteBatch() : mBufferObject(0), mArrayObject(0)
{
}

SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::begin(QuadSortType sortType ) 
{
	mSortType = sortType;
	mRenderBatches.clear();
	mQuads.clear();
}

void SpriteBatch::end() 
{
	mQuadPointers.resize(mQuads.size());
	for (unsigned int i = 0; i < mQuads.size(); i++)
	{
		mQuadPointers[i] = &mQuads[i];
	}

	sortQuads();
	createRenderBatches();
}

void SpriteBatch::addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour) 
{
	mQuads.emplace_back(destQuad, texCoord, texture, depth, colour);
}

void SpriteBatch::addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour, float angle)
{
	mQuads.emplace_back(destQuad, texCoord, texture, depth, colour, angle);
}

//void SpriteBatch::addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour, const glm::vec2& dir) {
//	const glm::vec2 right(1.0f, 0.0f);
//	float angle = acos(glm::dot(right, dir));
//	if (dir.y < 0.0f) angle = -angle;
//
//	mQuads.emplace_back(destQuad, texCoord, texture, depth, colour, angle);
//}

void SpriteBatch::renderBatches() 
{
	glBindVertexArray(mArrayObject);

	for (unsigned int i = 0; i < mRenderBatches.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
		glDrawArrays(GL_TRIANGLES, mRenderBatches[i].offset, mRenderBatches[i].numVertices);
	}

	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() 
{
	//Vertices for the quad
	std::vector<Vertex> vertices;

	//Resize the buffer to the exact size we need so we can treat
	//it like an array
	vertices.resize(mQuads.size() * QUAD_VERTICES);

	if (mQuads.empty())
	{
		return;
	}

	//Track the current vertex
	int currentVertex = 0;

	//Add the first batch
	mRenderBatches.emplace_back(currentVertex, QUAD_VERTICES, mQuadPointers[0]->texture);
	vertices[currentVertex++] = mQuadPointers[0]->topLeft;
	vertices[currentVertex++] = mQuadPointers[0]->bottomLeft;
	vertices[currentVertex++] = mQuadPointers[0]->bottomRight;
	vertices[currentVertex++] = mQuadPointers[0]->bottomRight;
	vertices[currentVertex++] = mQuadPointers[0]->topRight;
	vertices[currentVertex++] = mQuadPointers[0]->topLeft;

	//Add all the rest of the quads
	for (unsigned int i = 1; i < mQuadPointers.size(); i++) {

		//if this quad can't be part of the current batch (different textures)
		if (mQuadPointers[i]->texture != mQuadPointers[i - 1]->texture) 
		{
			//Make a new batch
			mRenderBatches.emplace_back(currentVertex, QUAD_VERTICES, mQuadPointers[i]->texture);
		}
		//if this quad can be part of the current batch (same textures)
		else 
		{
			//Increment numVertices by QUAD_VERTICES (6)
			mRenderBatches.back().numVertices += QUAD_VERTICES;
		}
		vertices[currentVertex++] = mQuadPointers[i]->topLeft;
		vertices[currentVertex++] = mQuadPointers[i]->bottomLeft;
		vertices[currentVertex++] = mQuadPointers[i]->bottomRight;
		vertices[currentVertex++] = mQuadPointers[i]->bottomRight;
		vertices[currentVertex++] = mQuadPointers[i]->topRight;
		vertices[currentVertex++] = mQuadPointers[i]->topLeft;
	}

	//Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	//Orphan the buffer and upload the vertice
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	//Unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//Generate VBO and VAO to initialise the sprite batch
void SpriteBatch::bufferData() 
{
	if (mArrayObject == 0) 
	{
		glGenVertexArrays(1, &mArrayObject);
	}

	//Bind the VAO
	glBindVertexArray(mArrayObject);

	if (mBufferObject == 0) 
	{
		glGenBuffers(1, &mBufferObject);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	glEnableVertexAttribArray(VERTEX_BUFFER);
	glVertexAttribPointer(VERTEX_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(COLOUR_BUFFER);
	glVertexAttribPointer(COLOUR_BUFFER, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	
	glEnableVertexAttribArray(TEXTURE_BUFFER);
	glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glBindVertexArray(0);

}

//Sort the quads by the chosen sort type
void SpriteBatch::sortQuads() 
{
	switch (mSortType)
	{
	case NONE:
		return;

	case FRONT_TO_BACK:
		std::stable_sort(mQuadPointers.begin(), mQuadPointers.end(), compareBackToFront);

	case BACK_TO_FRONT:
		std::stable_sort(mQuadPointers.begin(), mQuadPointers.end(), compareFrontToBack);

	case TEXTURE:
		std::stable_sort(mQuadPointers.begin(), mQuadPointers.end(), compareTexture);
	}
}

//True if quad a has a smaller depth than quad b
bool SpriteBatch::compareFrontToBack(Quad* a, Quad* b)
{
	return (a->depth < b->depth);
}

//True if quad has a greater depth than quad b 
bool SpriteBatch::compareBackToFront(Quad* a, Quad* b)
{
	return (a->depth > b->depth);
}

//True if quad a has a smaller texture id than quad b
//Sorting by texture means that sprites with the same texture will be drawn one after the other
bool SpriteBatch::compareTexture(Quad* a, Quad* b)
{
	return (a->texture < b->texture);
}