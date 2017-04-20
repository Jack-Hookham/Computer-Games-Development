#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
	const Texture& texture, const float density, const float friction, const glm::vec4& texCoords, const bool fixedRotation)
{
	//Initialise the entity's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexture = texture;
	mTexCoords = texCoords;

	//Box body definition
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//Box shape definition
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	//Box fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	mFixture = mBody->CreateFixture(&fixtureDef);
}

//Add Entity to SpriteBatch
void Entity::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - mDimensions.y / 2.0f);

	if (camera.isOnCamera(position, mDimensions))
	{
		spriteBatch.addSprite(position, mDimensions, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
	}
}
