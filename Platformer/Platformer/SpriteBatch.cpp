#include "SpriteBatch.h"

SpriteBatch::SpriteBatch()
{
}

SpriteBatch::~SpriteBatch()
{
}

//Prepare the sprite batch for rendering a new batch
void SpriteBatch::begin(SpriteSortType sortType) 
{
	mSortType = sortType;
	mRenderBatches.clear();
	mSprites.clear();
}

//Finish render preparation
//After this renderBatches() can be called
void SpriteBatch::end() 
{
	//Setup the sprite batch for sorting
	mSpritePointers.resize(mSprites.size());
	for (int i = 0; i < mSprites.size(); i++)
	{
		mSpritePointers[i] = &mSprites[i];
	}

	//Sort the sprite batch by the given sort type (default = TEXTURE)
	sortSprites();

	createRenderBatches();
}

//Add the sprite to the sprite batch
//No angle
void SpriteBatch::addSprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour)
{
	mSprites.emplace_back(position, dimensions, texCoord, texture, depth, colour);
}

//Add the sprite to the sprite batch
//With angle
void SpriteBatch::addSprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture, float depth, const Colour& colour, float angle)
{
	mSprites.emplace_back(position, dimensions, texCoord, texture, depth, colour, angle);
}

//Render the renderBatches in the sprite batch
void SpriteBatch::renderBatches() 
{
	//Bind the VAO
	glBindVertexArray(mArrayObject);

	for (int i = 0; i < mRenderBatches.size(); i++)
	{
		//Bind the texture to the target GL_TEXTURE_2D
		glBindTexture(GL_TEXTURE_2D, mRenderBatches[i].texture);
		//Render triangle primitives from the 
		glDrawArrays(GL_TRIANGLES, mRenderBatches[i].offset, mRenderBatches[i].numVertices);
	}

	//Unbind the VAO
	glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches() 
{
	//Vertices for the sprite
	std::vector<Vertex> vertices;

	//Resize the buffer to the exact size we need so we can treat
	//it like an array
	vertices.resize(mSprites.size() * SPRITE_VERTICES);

	if (mSprites.empty())
	{
		return;
	}

	//Track the current vertex
	int currentVertex = 0;

	//Add the first batch
	mRenderBatches.emplace_back(currentVertex, SPRITE_VERTICES, mSpritePointers[0]->getTexure());
	vertices[currentVertex++] = mSpritePointers[0]->getTL();
	vertices[currentVertex++] = mSpritePointers[0]->getBL();
	vertices[currentVertex++] = mSpritePointers[0]->getBR();
	vertices[currentVertex++] = mSpritePointers[0]->getBR();
	vertices[currentVertex++] = mSpritePointers[0]->getTR();
	vertices[currentVertex++] = mSpritePointers[0]->getTL();

	//Add all the rest of the sprites
	for (int i = 1; i < mSpritePointers.size(); i++) {

		//if this sprite can't be part of the current batch (different textures)
		if (mSpritePointers[i]->getTexure() != mSpritePointers[i - 1]->getTexure())
		{
			//Make a new batch
			mRenderBatches.emplace_back(currentVertex, SPRITE_VERTICES, mSpritePointers[i]->getTexure());
		}
		//if this sprite can be part of the current batch (same textures)
		else 
		{
			//Increment numVertices by SPRITE_VERTICES (6)
			mRenderBatches.back().numVertices += SPRITE_VERTICES;
		}
		vertices[currentVertex++] = mSpritePointers[i]->getTL();
		vertices[currentVertex++] = mSpritePointers[i]->getBL();
		vertices[currentVertex++] = mSpritePointers[i]->getBR();
		vertices[currentVertex++] = mSpritePointers[i]->getBR();
		vertices[currentVertex++] = mSpritePointers[i]->getTR();
		vertices[currentVertex++] = mSpritePointers[i]->getTL();
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
	//Generate the VAO
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

	//Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	//Enable the required vertex attribute array
	glEnableVertexAttribArray(VERTEX_BUFFER);
	//Define the attribute data - index, size, type, normalised, stride, GLvoid* pointer
	glVertexAttribPointer(VERTEX_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(COLOUR_BUFFER);
	glVertexAttribPointer(COLOUR_BUFFER, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	
	glEnableVertexAttribArray(TEXTURE_BUFFER);
	glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	//Unbind the VAO
	glBindVertexArray(0);
}

//Sort the sprites by the chosen sort type
void SpriteBatch::sortSprites() 
{
	switch (mSortType)
	{
	case NONE:
		return;

	case FRONT_TO_BACK:
		std::stable_sort(mSpritePointers.begin(), mSpritePointers.end(), compareBackToFront);

	case BACK_TO_FRONT:
		std::stable_sort(mSpritePointers.begin(), mSpritePointers.end(), compareFrontToBack);

	case TEXTURE:
		std::stable_sort(mSpritePointers.begin(), mSpritePointers.end(), compareTexture);
	}
}

//True if sprite a has a smaller depth than sprite b
bool SpriteBatch::compareFrontToBack(Sprite* a, Sprite* b)
{
	return (a->getDepth() < b->getDepth());
}

//True if sprite has a greater depth than sprite b 
bool SpriteBatch::compareBackToFront(Sprite* a, Sprite* b)
{
	return (a->getDepth() > b->getDepth());
}

//True if sprite a has a smaller texture id than sprite b
//Sorting by texture means that sprites with the same texture will be drawn one after the other
bool SpriteBatch::compareTexture(Sprite* a, Sprite* b)
{
	return (a->getTexure() < b->getTexure());
}