#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "SpriteBatch.h"
#include "TextureCache.h"
#include "ResourceManager.h"

class Box;
class Player;

class Entity
{
public:
	Entity();
	~Entity();

	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
		const Texture& texture, const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, bool fixedRotation = false);

	b2Body* getBody() const { return mBody; };
	b2Fixture* getFixture() const { return mFixture; };
	const glm::vec2& getDimensions() const { return mDimensions; }
	const Colour getColour() { return mColour; }

	void draw(SpriteBatch& spriteBatch);

protected:
	b2Body* mBody = NULL;
	b2Fixture* mFixture = NULL;

	glm::vec2 mPosition;
	glm::vec2 mDimensions;
	Colour mColour;
	Texture mTexture;
	glm::vec4 mTexCoords;
};

