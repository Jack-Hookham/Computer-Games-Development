#include "Sprite.h"

Sprite::Sprite() : mBufferObject(0)
{
}


Sprite::~Sprite()
{
	if (mBufferObject != 0)
	{
		glDeleteBuffers(1, &mBufferObject);
	}
}

void Sprite::init(float x, float y, float width, float height, std::string texturePath)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;
	mTexture = ResourceManager::getTexture(texturePath);

	if (mBufferObject == 0)
	{
		glGenBuffers(1, &mBufferObject);
	}

	//Create a vertex struct to hold the vertex data for a quad
	Vertex vertices[6];

	//First triangle
	vertices[0].setPosition(x + width, y + height);
	vertices[1].setPosition(x, y + height);
	vertices[2].setPosition(x, y);

	vertices[0].setTexCoord(1.0f, 1.0f);
	vertices[1].setTexCoord(0.0f, 1.0f);
	vertices[2].setTexCoord(0.0f, 0.0f);

	//Second triangle
	vertices[3].setPosition(x, y);
	vertices[4].setPosition(x + width, y);
	vertices[5].setPosition(x + width, y + height);

	vertices[3].setTexCoord(0.0f, 0.0f);
	vertices[4].setTexCoord(1.0f, 0.0f);
	vertices[5].setTexCoord(1.0f, 1.0f);

	//Set the colour of each vertex
	for (int i = 0; i < 6; i++)
	{
		vertices[i].setColour(255, 0, 255, 255);
	}

	vertices[1].setColour(128, 255, 0, 255);
	vertices[4].setColour(0, 128, 56, 255);

	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Draw the sprite on the screen
void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, mTexture.id);

	glBindBuffer(GL_ARRAY_BUFFER, mBufferObject);

	glEnableVertexAttribArray(VERTEX_BUFFER);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	
	glEnableVertexAttribArray(COLOUR_BUFFER);
	glVertexAttribPointer(COLOUR_BUFFER, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));

	glEnableVertexAttribArray(TEXTURE_BUFFER);
	glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(VERTEX_BUFFER);
	glDisableVertexAttribArray(COLOUR_BUFFER);
	glDisableVertexAttribArray(TEXTURE_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
