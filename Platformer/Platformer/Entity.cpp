#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour,
	const Texture& texture, const glm::vec4& texCoords, bool fixedRotation)
{
	//Initialise the entity's variables
	mPosition = position;
	mDimensions = dimensions;
	mColour = colour;
	mTexture = texture;
	mTexCoords = texCoords;

	//box2D body definition
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;
	mBody = world->CreateBody(&bodyDef);

	//box2D shape definition
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

	//box2D fixture definition
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.0f;
	mFixture = mBody->CreateFixture(&fixtureDef);
}

//Add Entity to SpriteBatch
void Entity::add(SpriteBatch& spriteBatch, Camera& camera)
{
	glm::vec2 position = glm::vec2(mBody->GetPosition().x - mDimensions.x / 2.0f, mBody->GetPosition().y - mDimensions.y / 2.0f);

	if (camera.isOnCamera(position, mDimensions))
	{
		spriteBatch.addQuad(position, mDimensions, mTexCoords, mTexture.id, 0.0f, mColour, mBody->GetAngle());
	}
}
