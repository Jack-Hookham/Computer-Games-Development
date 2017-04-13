#pragma once

#include <GL/glew.h>

struct Position
{
	float x;
	float y;
};

struct Colour 
{
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

	void setTexCoord(float x, float y)
	{
		texCoord.x = x;
		texCoord.y = y;
	}
};