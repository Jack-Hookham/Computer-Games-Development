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

	void init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions);

	b2Body* getBody() const { return mBody; };
	b2Fixture* getFixture() const { return mFixture; };
	const glm::vec2& getDimensions() const { return mDimensions; }

	void draw(SpriteBatch& spriteBatch);

private:
	b2Body* mBody = NULL;
	b2Fixture* mFixture = NULL;

	glm::vec2 mDimensions;
};

