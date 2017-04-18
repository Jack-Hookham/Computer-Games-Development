#pragma once

#include "Box.h"
#include "InputManager.h"

class Player
{
public:
	Player();
	~Player();

	void init(b2World* world, const glm::vec2& position, const glm::vec2& dimensions, Colour& color);

	void update(InputManager& inputManager);

	void draw(SpriteBatch& spriteBatch);

	const Box& getBox() const { return mCollisionBox; }

private:
	Box mCollisionBox;
};

