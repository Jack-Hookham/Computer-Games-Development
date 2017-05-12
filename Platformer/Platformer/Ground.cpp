#include "Ground.h"

Ground::Ground()
{
}


Ground::~Ground()
{
	Entity::~Entity();
}

void Ground::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
	const Texture& texture, const float friction, const glm::vec4& texCoords, const bool fixedRotation)
{
	//Initialise the entity's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexture = texture;
	mTexCoords = texCoords;

	//Ground body definition
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(position.x, position.y);
	mBody = world->CreateBody(&groundBodyDef);

	//Ground fixture definition
	b2PolygonShape groundBox;
	groundBox.SetAsBox(dimensions.x, dimensions.y);
	//The ground is static so its density is always 0
	mBody->CreateFixture(&groundBox, 0.0f);
}

//Add ground to sprite batch
void Ground::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x, mBody->GetPosition().y - mDimensions.y);
	glm::vec2 dimensions = glm::vec2(mDimensions.x * 2.0f, mDimensions.y * 2.0f);

	if (camera.isOnCamera(position, dimensions))
	{
		spriteBatch.addSprite(position, dimensions, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
	}
}