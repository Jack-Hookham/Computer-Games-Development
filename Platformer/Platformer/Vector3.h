/*
Implementation of a 3D vector created for CSC3221 - Programming for Games
Using 3D rather than 2D so that depth values can be stored
*/

#pragma once
#include <iostream>

using namespace::std;

class Vector3
{
public:

	Vector3();
	Vector3(const float x, const float y, const float z);
	Vector3(const float x, const float y);
	Vector3(Vector3& v);

	~Vector3();

	float getX() const;
	float getY() const;
	float getZ() const;

	//Using inline speeds up the program by avoiding function calling overhead
	//cout is used a lot during testing so this improves performance
	inline friend ostream& operator<<(ostream& out, const Vector3& v)
	{
		out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return out;
	}

	friend istream& operator >> (istream& in, Vector3& v)
	{
		in >> v.x >> v.y >> v.z;
		return in;
	}

	float magnitude() const;
	float magnitudeSquared() const;

	Vector3 operator+(const Vector3& v) const;
	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(const float s) const;
	Vector3 operator/(const float s) const;

	Vector3 operator+=(const Vector3 & v);
	Vector3 operator-=(const Vector3 & v);
	Vector3 operator*=(const float s);
	Vector3 operator/=(const float s);

	float operator*(const Vector3& v) const;
	Vector3 operator%(const Vector3& v) const;

	Vector3 unitVector() const;

	Vector3 operator=(const Vector3& v);

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	bool operator>(const Vector3& v) const;
	bool operator<(const Vector3& v) const;

	Vector3 invert();
	Vector3 inverse() const;

	float maxElement() const;
	float minElement() const;

private:

	float x;
	float y;
	float z;
};

