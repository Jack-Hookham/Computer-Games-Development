#include "Box.h"

Box::Box()
{
}

Box::~Box()
{
}

void Box::update()
{
	EntityBox2D::update();
	mAngle = mBody->GetAngle();
}
