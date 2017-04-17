#pragma once

#include <GL/glew.h>

struct Position
{
	float x;
	float y;
};

struct Colour 
{
	Colour() : r(0), g(0), b(0), a(255) { }
	Colour(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : 
		r(R), g(G), b(B), a(A) { }

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