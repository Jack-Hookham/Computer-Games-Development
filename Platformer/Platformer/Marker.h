#pragma once

#include "Entity.h"

//Marker for Enemy logic
class Marker : public Entity
{
public:
	Marker();
	~Marker();

	//Initialise the entity
	void init(const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture,
		const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, const bool fixedRotation = true);

	//Cull then add Entity to SpriteBatch
	void add(SpriteBatch& spriteBatch, Camera& camera);

	const glm::vec2& getPosition() const { return mPosition; }
	const glm::vec2& getDimensions() const { return mDimensions; }
	const Colour& getColour() const { return mColour; }

};

