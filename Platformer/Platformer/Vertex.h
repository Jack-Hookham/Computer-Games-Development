//Contains Position, Colour and TexCoord structs
//These structs make up a Vertex struct

#pragma once

#include <GL/glew.h>

struct Position
{
	float x;
	float y;
};

struct Colour 
{
	Colour()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 255;
	}

	Colour(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{ 
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Colour(glm::vec4 col)
	{
		r = (GLubyte)col.x;
		g = (GLubyte)col.y;
		b = (GLubyte)col.z;
		a = (GLubyte)col.w;
	}

	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct TexCoord
{
	float x;
	float y;
};

struct Vertex
{
	Position position;
	Colour colour;
	TexCoord texCoord;

	void setPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		colour.r = r;
		colour.g = g;
		colour.b = b;
		colour.a = a;
	}

	void setColour(Colour colour)
	{
		this->colour = colour;
	}

	void setTexCoord(float x, float y)
	{
		texCoord.x = x;
		texCoord.y = y;
	}
};