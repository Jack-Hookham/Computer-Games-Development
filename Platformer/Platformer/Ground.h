#pragma once

#include "Entity.h"

//Used to create ground in the world
//Density should usually be 0.0f means it isn't effected by physics in the b2World

class Ground : public Entity
{
public:
	Ground();
	~Ground();

	virtual void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture,
		const float density = 0.0f, const float friction = 0.3f, const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		const bool fixedRotation = true);

	virtual void add(SpriteBatch& spriteBatch, Camera& camera);
};

