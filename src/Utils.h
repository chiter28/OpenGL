#pragma once
#include <cmath>
#include <numbers>
#include <random>

#define DegtoRad(degrees) ((degrees) * std::numbers::pi / 180.0f)

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

	mat4 operator* (const mat4& other) const
	{
		mat4 result;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result(row, col) = 
					operator()(row, 0) * other(0, col) +
					operator()(row, 1) * other(1, col) +
					operator()(row, 2) * other(2, col) +
					operator()(row, 3) * other(3, col);
			}
		}
		return result;
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

	static mat4 scale(const vec3& vector)
	{
		mat4 result;
		result[0] = vector.x;
		result[5] = vector.y;
		result[10] = vector.z;
		return result;
	}

		// Column - Major(по столбцам)
		// [ m[0]  m[4]  m[8]   m[12]
		// [ m[1]  m[5]  m[9]   m[13]
		// [ m[2]  m[6]  m[10]  m[14]
		// [ m[3]  m[7]  m[11]  m[15]
	static mat4 perspective(float fovDegrees, float aspectRatio, float zNear, float zFar)
	{
		mat4 result;
		for (int i = 0; i < 16; ++i) result[i] = 0.0f;

		float tanHalfFOV = std::tan(DegtoRad(fovDegrees / 2.0f)); // tg(Alpha/2)

		result[0] = 1.0f / (aspectRatio * tanHalfFOV);
		result[5] = 1.0f / tanHalfFOV;

		// Z-mapping в NDC [-1, 1]
		result[10] = -(zFar + zNear) / (zFar - zNear);

		// Смещение по Z 
		result[14] = -(2.0f * zFar * zNear) / (zFar - zNear);

		// Запись -Z в W-компоненту для перспективного деления
		result[11] = -1.0f;
		
		return result;
	}


	static mat4 camera(vec3 cameraPosition, vec3 u, vec3 v, vec3 n)
	{
		mat4 result;

		result[0] = u.x;
		result[1] = v.x;
		result[2] = n.x;

		result[4] = u.y;
		result[5] = v.y;
		result[6] = n.y;
		
		result[8]  = u.z;
		result[9]  = v.z;
		result[10] = n.z;

		result[12] = -u.x * cameraPosition.x - u.y * cameraPosition.y - u.z * cameraPosition.z;
		result[13] = -v.x * cameraPosition.x - v.y * cameraPosition.y - v.z * cameraPosition.z;
		result[14] = -n.x * cameraPosition.x - n.y * cameraPosition.y - n.z * cameraPosition.z;
	
		return result;
	}

}; 







