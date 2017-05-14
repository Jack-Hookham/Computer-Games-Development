#include "EntityBox2D.h"

EntityBox2D::EntityBox2D()
{
}

EntityBox2D::~EntityBox2D()
{
	mBody->GetWorld()->DestroyBody(mBody);
}

void EntityBox2D::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
	const Texture& texture, const float density, const float friction, const glm::vec4& texCoords, const bool fixedRotation)
{
	Entity::init(position, dimensions, colour, texture, texCoords);

	//Box body definition
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(mPosition.x, mPosition.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//Box shape definition
	b2PolygonShape boxShape;
	boxShape.SetAsBox(mDimensions.x * 0.5f, mDimensions.y * 0.5f);

	//Box fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	mFixture = mBody->CreateFixture(&fixtureDef);
}

void EntityBox2D::update()
{
	mPosition.x = mBody->GetPosition().x;
	mPosition.y = mBody->GetPosition().y;
}

void EntityBox2D::add(SpriteBatch & spriteBatch, Camera & camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x * 0.5f, mBody->GetPosition().y - mDimensions.y * 0.5f);

	if (camera.isOnCamera(position, mDimensions))
	{
		spriteBatch.addSprite(position, mDimensions, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
	}
}
