#include "Bullet.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float speed, int lifeTime)
{
	mSpeed = speed;
	mPosition = position;
	mDirection = direction;
	mLifeTime = lifeTime;
}

Bullet::~Bullet()
{
}

//void Bullet::init(float speed, glm::vec2 position, glm::vec2 direction, int lifeTime)
//{
//	mSpeed = speed;
//	mPosition = position;
//	mDirection = direction;
//	mLifeTime = lifeTime;
//}

void Bullet::draw(SpriteBatch& spriteBatch)
{
	glm::vec4 dimensions(mPosition.x, mPosition.y, 30.0f, 30.0f);
	glm::vec4 texCoords(0.0f, 0.0f, 1.0f, 1.0f);
	static GLTexture texture = ResourceManager::getTexture("../res/textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
	Colour colour(255, 255, 255, 255);

	spriteBatch.addQuad(dimensions, texCoords, texture.id, 0.0f, colour);
}

bool Bullet::update(float timeStep)
{
	mPosition += mDirection * mSpeed * timeStep;
	mLifeTime--;

	if (mLifeTime == 0)
	{
		return true;
	}
	return false;
}
