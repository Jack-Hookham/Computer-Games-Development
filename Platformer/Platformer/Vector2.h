/*
Implementation of a 3D vector created for CSC3221 - Programming for Games
Using 3D rather than 2D so that depth values can be stored
*/

#pragma once
#include <iostream>

using namespace::std;

class Vector2
{
public:

	Vector2();
	Vector2(const float x, const float y);
	Vector2(Vector2& v);

	~Vector2();

	float getX() const;
	float getY() const;

	//Using inline speeds up the program by avoiding function calling overhead
	//cout is used a lot during testing so this improves performance
	inline friend ostream& operator<<(ostream& out, const Vector2& v)
	{
		out << "(" << v.x << ", " << v.y << ")";
		return out;
	}

	friend istream& operator >> (istream& in, Vector2& v)
	{
		in >> v.x >> v.y;
		return in;
	}

	float magnitude() const;
	float magnitudeSquared() const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const float s) const;
	Vector2 operator/(const float s) const;

	Vector2 operator+=(const Vector2 & v);
	Vector2 operator-=(const Vector2 & v);
	Vector2 operator*=(const float s);
	Vector2 operator/=(const float s);

	float operator*(const Vector2& v) const;

	Vector2 unitVector() const;

	Vector2 operator=(const Vector2& v);

	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	bool operator>(const Vector2& v) const;
	bool operator<(const Vector2& v) const;

	Vector2 invert();
	Vector2 inverse() const;

	float maxElement() const;
	float minElement() const;

private:

	float x;
	float y;
};

