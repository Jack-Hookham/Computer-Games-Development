#include "Box.h"

Box::Box()
{
}


Box::~Box()
{
}

void Box::init(b2World* world, const glm::vec2 &position, const glm::vec2 &dimensions)
{
	mDimensions = dimensions;

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
	mFixture = mBody->CreateFixture(&fixtureDef);
}

void Box::draw(SpriteBatch &spriteBatch)
{
	glm::vec4 dimensions(mBody->GetPosition().x, mBody->GetPosition().y, 30.0f, 30.0f);
	glm::vec4 texCoords(0.0f, 0.0f, 1.0f, 1.0f);
	static GLTexture texture = ResourceManager::getTexture("../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Colour colour(255, 255, 255, 255);

	spriteBatch.addQuad(dimensions, texCoords, texture.id, 0.0f, colour);
}
