#include "Vector3.h"
#include <cmath>

//Default constructor
Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

//Constructor with x, y, x params
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//Constructor with x and y params
Vector3::Vector3(float x, float y)
{
	this->x = x;
	this->y = y;
	z = 0;
}

//Constructor copy an existing vector
Vector3::Vector3(Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

//Destructor
Vector3::~Vector3() {}

//x getter
float Vector3::getX() const
{
	return x;
}

//y getter
float Vector3::getY() const
{
	return y;
}

//z getter
float Vector3::getZ() const
{
	return z;
}

//Magnitude of this vector
float Vector3::magnitude() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

//Magnitude squared - for performance when actual magnitude is not needed
//for example in greater than/less than it means the expensive sqrt doesn't need to be used
float Vector3::magnitudeSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

//Vector addition
Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

//Vector subtraction
Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

//Vector multiplication by a scalar
Vector3 Vector3::operator*(const float s) const
{
	return Vector3(x * s, y * s, z * s);
}

//Vector division by a scalar
Vector3 Vector3::operator/(const float s) const
{
	//Check for divide by 0
	if (s == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}
	return Vector3(x / s, y / s, z / s);
}

//Vector += operator
Vector3 Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

//Vector -= operator
Vector3 Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

//Vector *= operator
Vector3 Vector3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

//Vector /= operator
Vector3 Vector3::operator/=(const float s)
{
	//Check for divide by 0
	if (s == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}

	x /= s;
	y /= s;
	z /= s;

	return *this;
}

//Scalar/Dot product
float Vector3::operator*(const Vector3& v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

//Vector/Cross product
Vector3 Vector3::operator%(const Vector3& v) const
{
	return Vector3(
		(y * v.z) - (z * v.y),
		(z * v.x) - (x * v.z),
		(x * v.y) - (y * v.x));
}

//Unit vector (of magnitude 1)
Vector3 Vector3::unitVector() const
{
	float m = magnitude();

	//Check for divide by 0
	if (m == 0)
	{
		throw std::invalid_argument("Divide by zero exception");
	}
	return Vector3(x / m, y / m, z / m);
}

//Assignment
Vector3 Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

//Equality
bool Vector3::operator==(const Vector3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

//Inequality
bool Vector3::operator!=(const Vector3& v) const
{
	return !(x == v.x && y == v.y && z == v.z);
}

//Greater than
bool Vector3::operator>(const Vector3& v) const
{
	return (magnitudeSquared() > v.magnitudeSquared());
}

//Less than
bool Vector3::operator<(const Vector3& v) const
{
	return (magnitudeSquared() < v.magnitudeSquared());
}

//Additive inverse of this vector
Vector3 Vector3::invert()
{
	x = -x;
	y = -y;
	z = -z;

	return *this;
}

//Additive inverse returning a new Vector
Vector3 Vector3::inverse() const
{
	return Vector3(-x, -y, -z);
}

//Get maximum element of a vector
float Vector3::maxElement() const
{
	return fmax(x, fmax(y, z));
}

//Get minimum element of a vector
float Vector3::minElement() const
{
	return fmin(x, fmin(y, z));
}