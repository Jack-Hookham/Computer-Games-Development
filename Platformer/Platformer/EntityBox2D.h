#pragma once

#include "Entity.h"

//Entity with box2D physics

class Box;
class Ground;
class Player;
class Enemy;
class Marker;

class EntityBox2D : public Entity
{
public:
	EntityBox2D();
	~EntityBox2D();	
	
	//Initialise the entity
	virtual void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture,
		const float density = 1.0f, const float friction = 0.3f, const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f },
		const bool fixedRotation = false);

	virtual void update();
	
	//Cull then add Entity to SpriteBatch
	virtual void add(SpriteBatch& spriteBatch, Camera& camera);

	inline const b2Body* getBody() const { return mBody; };
	inline const b2Fixture* getFixture() const { return mFixture; };

protected:

	//box2D stuff
	b2Body* mBody = NULL;
	b2Fixture* mFixture = NULL;
};

