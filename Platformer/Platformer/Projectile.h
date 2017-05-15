#pragma once

#include "Entity.h"
#include "Timer.h"

//Player thrown weapon
//Not effected by box2D physics
class Projectile : public Entity
{
public:
	Projectile();
	~Projectile();

	void init(const glm::vec2& position, const glm::vec2& dimensions, const Colour& colour, const Texture& texture,
		const glm::vec2& velocity, const glm::vec4& texCoords = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f });

	void update();

	inline Timer getLifeTimer() const { return mLifeTimer; }
	
	inline void setVelocity(const glm::vec2 velocity) { mVelocity = velocity; }

private:
	glm::vec2 mVelocity;
	Timer mLifeTimer;
};

