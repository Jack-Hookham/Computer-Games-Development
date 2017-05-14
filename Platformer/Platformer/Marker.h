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
		const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	//Cull then add Entity to SpriteBatch
	void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const void setPosition(const glm::vec2 position) { mPosition = position; }
	inline const void setPosition(const float xPos, const float yPos) { mPosition.x = xPos; mPosition.y = yPos; }
	inline const void setDimensions(const float xDim, const float yDim) { mDimensions.x = xDim; mDimensions.y = yDim; }
};

