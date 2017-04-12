#include "Vector2.h"
#include <cmath>

//Default constructor
Vector2::Vector2()
{
	x = 0;
	y = 0;
}

//Constructor with x, y, x params
Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

//Constructor copy an existing vector
Vector2::Vector2(Vector2& v)
{
	x = v.x;
	y = v.y;
}

//Destructor
Vector2::~Vector2() {}

//x getter
float Vector2::getX() const
{
	return x;
}

//y getter
float Vector2::getY() const
{
	return y;
}

//x setter
void Vector2::setX(float x)
{
	this->x = x;
}

//y setter
void Vector2::setY(float y)
{
	this->y = y;
}

//Magnitude of this vector
float Vector2::magnitude() const
{
	return sqrt((x * x) + (y * y));
}

//Magnitude squared - for performance when actual magnitude is not needed
//for example in greater than/less than it means the expensive sqrt doesn't need to be used
float Vector2::magnitudeSquared() const
{
	return (x * x) + (y * y);
}

//Vector addition
Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

//Vector subtraction
Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

//Vector multiplication by a scalar
Vector2 Vector2::operator*(const float s) const
{
	return Vector2(x * s, y * s);
}

//Vector division by a scalar
Vector2 Vector2::operator/(const float s) const
{
	//Check for divide by 0
	if (s == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}
	return Vector2(x / s, y / s);
}

//Vector += operator
Vector2 Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

//Vector -= operator
Vector2 Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

//Vector *= operator
Vector2 Vector2::operator*=(const float s)
{
	x *= s;
	y *= s;

	return *this;
}

//Vector /= operator
Vector2 Vector2::operator/=(const float s)
{
	//Check for divide by 0
	if (s == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}

	x /= s;
	y /= s;

	return *this;
}

//Scalar/Dot product
float Vector2::operator*(const Vector2& v) const
{
	return (x * v.x) + (y * v.y);
}

//Unit vector (of magnitude 1)
Vector2 Vector2::unitVector() const
{
	float m = magnitude();

	//Check for divide by 0
	if (m == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}
	return Vector2(x / m, y / m);
}

//Assignment
Vector2 Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;

	return *this;
}

//Equality
bool Vector2::operator==(const Vector2& v) const
{
	return (x == v.x && y == v.y);
}

//Inequality
bool Vector2::operator!=(const Vector2& v) const
{
	return !(x == v.x && y == v.y);
}

//Greater than
bool Vector2::operator>(const Vector2& v) const
{
	return (magnitudeSquared() > v.magnitudeSquared());
}

//Less than
bool Vector2::operator<(const Vector2& v) const
{
	return (magnitudeSquared() < v.magnitudeSquared());
}

//Additive inverse of this vector
Vector2 Vector2::invert()
{
	x = -x;
	y = -y;

	return *this;
}

//Additive inverse returning a new Vector
Vector2 Vector2::inverse() const
{
	return Vector2(-x, -y);
}

//Get maximum element of a vector
float Vector2::maxElement() const
{
	return fmax(x, y);
}

//Get minimum element of a vector
float Vector2::minElement() const
{
	return fmin(x, y);
}