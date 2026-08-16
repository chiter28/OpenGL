#include "Utils.h"


vec3::vec3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{}

vec3::vec3(float f)
	: x(f), y(f), z(f)
{}

float vec3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

vec3 vec3::operator+ (vec3 other)
{
	return vec3{ x + other.x, y + other.y, z + other.z };
}

vec3 vec3::operator- () const
{
	return vec3(-x, -y, -z);
}


vec3& vec3::operator+= (vec3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
};

vec3& vec3::operator-= (vec3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
};


vec3 vec3::operator* (vec3 other)
{
	return vec3{ x * other.x, y * other.y, z * other.z };
}

vec3 vec3::operator* (float number)
{
	return vec3{ x * number, y * number, z * number };
}

vec3 vec3::normalized() const
{
	float len = length();

	if (len > 0.0f) {
		return vec3(x / len, y / len, z / len);
	}
	return vec3(0.0f, 0.0f, 0.0f);
}

vec3& vec3::normalized()
{
	float len = length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
	return *this;
}

float vec3::dot(const vec3& b) const
{
	return x * b.x + y * b.y + z * b.z;
}

vec3 vec3::cross(const vec3& other) const
{
	return vec3(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

vec3& vec3::rotate(float angle, const vec3& axis)
{
	Quat Rotation = Quat::fromAngleAxis(angle, axis);
	Quat Conjugate = Rotation.Conjugate();
	Quat W = Rotation * *this * Conjugate;
	x = W.V.x;
	y = W.V.y;
	z = W.V.z;

	return *this;
}