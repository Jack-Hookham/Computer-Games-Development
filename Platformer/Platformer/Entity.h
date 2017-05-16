#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "SpriteBatch.h"
#include "ResourceManager.h"
#include "Camera.h"

//The base entity class for the engine
//All entities inherit from this class

class EntityBox2D;
class Marker;
class Projectile;

class Entity
{
public:
	Entity();
	~Entity();

	//Initialise the entity
	virtual void init(const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture, 
		const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	//Cull then add Entity to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const glm::vec2& getPosition() const { return mPosition; }
	inline const glm::vec2& getDimensions() const { return mDimensions; }
	inline const Colour& getColour() const { return mColour; }
	inline const Texture& getTexture() const { return mTexture; }

protected:

	//Params
	glm::vec2 mPosition;
	glm::vec2 mDimensions;
	Colour mColour;
	Texture mTexture;
	glm::vec4 mTexCoords;
};

