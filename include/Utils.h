#pragma once
#include <cmath>

struct vec3
{
	float x, y, z;

public:
	vec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		: x(_x), y(_y), z(_z) {}
	
	vec3(float f)
		: x(f), y(f), z(f) {}

	float length() {
		return std::sqrt(x * x + y * y + z * z);
	}

	static float dot(const vec3& a, const vec3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

};



struct mat4
{
	// Column - Major(по столбцам)
	// [ m[0]  m[4]  m[8]   m[12]
	// [ m[1]  m[5]  m[9]   m[13]
	// [ m[2]  m[6]  m[10]  m[14]
	// [ m[3]  m[7]  m[11]  m[15]

	float m[16];
public:

	mat4()
	{
		identity();
	}

	void identity()
	{
		for (int i = 0; i < 16; i++) {
			m[i] = 0.0f;
		}
		m[0] = m[5] = m[10] = m[15] = 1.0f;
	}

	float& operator[] (int index) {
		return m[index];
	}

	const float& operator[] (int index) const {
		return m[index];
	}

	float* value_ptr() { return m; }
	const float* value_ptr() const { return m; }

	float& operator() (int row, int col) { return m[col * 4 + row]; }
	const float& operator() (int row, int col) const { return m[col * 4 + row]; }

	mat4(float x, float y, float z)
	{
		identity();
		m[12] = x;
		m[13] = y;
		m[14] = z;
	}

	static mat4 translate(const vec3& v)
	{
		mat4 result;
		result[12] = v.x;
		result[13] = v.y;
		result[14] = v.z;
		return result;
	}

	static mat4 rotate(float angleRad, vec3 axis)
	{
		mat4 res;
		float cos = std::cos(angleRad);
		float sin = std::sin(angleRad);

		if (axis.x == 1.0f) {
			res[5] = cos;
			res[6] = sin;
			res[9] = -sin;
			res[10] = cos;
		}
		else if (axis.y == 1.0f) {
			res[0] = cos;
			res[2] = sin;
			res[8] = -sin;
			res[10] = cos;

		}
		else if (axis.z == 1.0f) {
			res[0] = cos;
			res[1] = sin;
			res[4] = -sin;
			res[5] = cos;
		}
		return res;
	}

};




