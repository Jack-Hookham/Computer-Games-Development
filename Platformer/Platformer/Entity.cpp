#include "Entity.h"

Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions,
	const Colour& colour, const GLTexture texture, const glm::vec4 texCoords)
{
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexture = texture;
	mTexCoords = texCoords;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	mBody = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;
	mFixture = mBody->CreateFixture(&fixtureDef);
}

void Entity::draw(SpriteBatch &spriteBatch)
{
	glm::vec4 destQuad;
	destQuad.x = mBody->GetPosition().x - mDimensions.x / 2.0f;
	destQuad.y = mBody->GetPosition().y - mDimensions.y / 2.0f;
	destQuad.z = mDimensions.x;
	destQuad.w = mDimensions.y;

	spriteBatch.addQuad(destQuad, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
}
