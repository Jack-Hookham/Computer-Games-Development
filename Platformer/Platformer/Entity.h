#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "SpriteBatch.h"
#include "ResourceManager.h"
#include "Camera.h"

//The base entity class for the engine
//All entities inherit from this class

class Box;
class Ground;
class Player;
class Enemy;

class Entity
{
public:
	Entity();
	~Entity();

	//Initialise the entity
	virtual void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture, 
		const float density = 1.0f, const float friction = 0.3f, const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f }, 
		const bool fixedRotation = false);

	const b2Body* getBody() const { return mBody; };
	const b2Fixture* getFixture() const { return mFixture; };

	const glm::vec2& getPosition() const { return mPosition; }
	const glm::vec2& getDimensions() const { return mDimensions; }
	const Colour& getColour() const { return mColour; }

	//Cull then add Entity to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

protected:
	//box2D stuff
	b2Body* mBody = NULL;
	b2Fixture* mFixture = NULL;

	//Params
	glm::vec2 mPosition;
	glm::vec2 mDimensions;
	Colour mColour;
	Texture mTexture;
	glm::vec4 mTexCoords;
};

