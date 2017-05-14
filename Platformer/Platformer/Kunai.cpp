#include "Kunai.h"

Kunai::Kunai()
{
}

Kunai::~Kunai()
{
}

void Kunai::init(const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture, 
	const glm::vec2& velocity, const glm::vec4& texCoords)
{
	Entity::init(position, dimensions, colour, texture, texCoords);
	mVelocity = velocity;
	mLifeTimer.start();
}

void Kunai::update()
{
	mPosition += mVelocity;
}
