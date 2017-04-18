#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "Entity.h"
#include "SpriteBatch.h"
#include "GLTexture.h"
#include "ResourceManager.h"

class Box : public Entity
{
public:
	Box();
	~Box();
};

