#include "SpriteBatch.h"

SpriteBatch::SpriteBatch()
{
}

SpriteBatch::~SpriteBatch()
{
	//deallocate quad pointers
	for (int i = 0; i < mQuads.size(); i++)
	{
		delete mQuadPointers[i];
	}
}

//Generate VBO and VAO to initialise the sprite batch
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

//Begin the spritebatch
void SpriteBatch::begin(QuadSortType sortType)
{
	mSortType = sortType;
	mRenderBatches.clear();
	mQuads.clear();
}

//End the spritebatch
void SpriteBatch::end()
{
	mQuadPointers.resize(mQuads.size());
	for (int i = 0; i < mQuads.size(); i++)
	{
		mQuadPointers[i] = &mQuads[i];
	}

	sortQuads();
	createRenderBatches();
}

//Adds quad containing information about an indivudual quad used in the SpriteBatch 
//for recTo and texCoord vec4s:
//x = x coord
//y = y coord
//z = width
//w = height
void SpriteBatch::addQuad(const glm::vec4& destQuad, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour)
{
	mQuads.emplace_back(destQuad, texCoord, texture, depth, colour);
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

void SpriteBatch::sortQuads()
{
	switch (mSortType)
	{
	case BACK_TO_FRONT:
		std::stable_sort(mQuadPointers.begin(), mQuadPointers.end(), compareFrontToBack);

	case FRONT_TO_BACK:
		std::stable_sort(mQuadPointers.begin(), mQuadPointers.end(), compareBackToFront);

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

void SpriteBatch::createRenderBatches()
{
	//Vertices for the quad
	std::vector<Vertex> vertices;
	//Allocate memory for the vector by resizing it to the size needed
	vertices.resize(mQuads.size() * QUAD_VERTICES);

	if (mQuads.empty())
	{
		return;
	}

	//current vertex
	int currentVertex = 0;

	//Add the first batch
	mRenderBatches.emplace_back(0, QUAD_VERTICES, mQuadPointers[0]->texture);
	vertices[currentVertex++] = mQuadPointers[0]->topLeft;
	vertices[currentVertex++] = mQuadPointers[0]->bottomLeft;
	vertices[currentVertex++] = mQuadPointers[0]->bottomRight;
	vertices[currentVertex++] = mQuadPointers[0]->bottomRight;
	vertices[currentVertex++] = mQuadPointers[0]->topRight;
	vertices[currentVertex++] = mQuadPointers[0]->topLeft;

	//Add the rest of the quads
	for (int i = 1; i < mQuadPointers.size(); i++)
	{
		//if this quad can't be part of the current batch (different textures)
		if (mQuadPointers[i]->texture != mQuadPointers[i - 1]->texture)
		{
			//Make a new batch
			mRenderBatches.emplace_back(currentVertex, QUAD_VERTICES, mQuadPointers[0]->texture);
		}
		//if this quad can be part of the current batch (same textures)
		else
		{
			//increase the number of vertices
			mRenderBatches.back().numVertices += QUAD_VERTICES;
		}
		vertices[currentVertex++] = mQuadPointers[i]->topLeft;
		vertices[currentVertex++] = mQuadPointers[i]->bottomLeft;
		vertices[currentVertex++] = mQuadPointers[i]->bottomRight;
		vertices[currentVertex++] = mQuadPointers[i]->bottomRight;
		vertices[currentVertex++] = mQuadPointers[i]->topRight;
		vertices[currentVertex++] = mQuadPointers[i]->topLeft;
	}

	//bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	//Orphan and upload the vertice data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

	//unbind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
