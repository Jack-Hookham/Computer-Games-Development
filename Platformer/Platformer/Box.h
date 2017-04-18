#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "SpriteBatch.h"
#include "GLTexture.h"
#include "ResourceManager.h"

class Box
{
public:
	Box();
	~Box();

	void init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions, const Colour& colour, 
		const GLTexture texture, const glm::vec4 texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	b2Body* getBody() const { return mBody; };
	b2Fixture* getFixture() const { return mFixture; };
	const glm::vec2& getDimensions() const { return mDimensions; }
	const Colour getColour() { return mColour; }

	void draw(SpriteBatch& spriteBatch);

private:
	b2Body* mBody = NULL;
	b2Fixture* mFixture = NULL;

	glm::vec2 mPosition;
	glm::vec2 mDimensions;
	Colour mColour;
	GLTexture mTexture;
	glm::vec4 mTexCoords;
};

