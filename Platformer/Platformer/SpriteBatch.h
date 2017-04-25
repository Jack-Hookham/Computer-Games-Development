#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "Sprite.h"

//SpriteBatch class used for managing and drawing the different sprites used in the engine
//Based on the sprite batch class used here: https://www.youtube.com/watch?v=jXKMfsiVkLA

enum BufferType
{
	VERTEX_BUFFER,
	COLOUR_BUFFER,
	TEXTURE_BUFFER,
};

//Type of sorting used for sorting the vector of sprites
enum SpriteSortType
{
	NONE,
	FRONT_TO_BACK,
	BACK_TO_FRONT,
	TEXTURE
};

//Each render batch is used for a single draw call
struct RenderBatch
{
	RenderBatch(GLuint offset, GLuint numVertices, GLuint texture)
	{
		this->offset = offset;
		this->numVertices = numVertices;
		this->texture = texture;
	}

	//The index offset for drawing the triangle primitives
	GLuint offset;
	//Number of vertices in the render batch = number of indices to be rendered
	GLuint numVertices;
	//The texture to use
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void bufferData();

	void begin(SpriteSortType sortType = TEXTURE);
	void end();

	void addSprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture,
		float depth, const Colour& colour);
	void addSprite(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& texCoord, GLuint texture,
		float depth, const Colour& colour, float angle);

	void renderBatches();

private:
	//2 trianges = 6 vertices
	const unsigned int SPRITE_VERTICES = 6;

	void sortSprites();

	//Sorting functions used for std::stable_sort in sortSprites()
	static bool compareFrontToBack(Sprite* a, Sprite* b);
	static bool compareBackToFront(Sprite* a, Sprite* b);
	static bool compareTexture(Sprite* a, Sprite* b);

	void createRenderBatches();

	//VBO for the sprite
	GLuint mBufferObject = 0;

	//VAO for the sprite
	GLuint mArrayObject = 0;

	//Sprites
	std::vector<Sprite> mSprites;
	//Sprite pointers for sorting
	std::vector<Sprite*> mSpritePointers;

	//RenderBatches
	std::vector<RenderBatch> mRenderBatches;

	//SortType
	SpriteSortType mSortType;
};

