#pragma once
#include <cmath>
#include <numbers>
#include <random>

#define DegtoRad(degrees) ((degrees) * std::numbers::pi / 180.0f)
#define RadtoDeg(radians) ((radians) * 180.0f / std::numbers::pi)











struct vec3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
		

public:
	vec3() = default;
	vec3(float _x, float _y, float _z);
	
	explicit vec3(float f);

	float length() const;

	vec3 operator+ (vec3 other);
	
	vec3 operator- () const;


	vec3& operator+= (vec3 other);

	vec3& operator-= (vec3 other);


	vec3 operator* (vec3 other);

	vec3 operator* (float number);
	
	vec3 normalized() const;

	vec3& normalized();

	float dot(const vec3& b) const;

	vec3 cross(const vec3& other) const;

	vec3& rotate(float angle, const vec3& axis);
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
		if (index < 16)
			return m[index];
		return -1;
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

	mat4& translate(const vec3& v)
	{
		m[12] = v.x;
		m[13] = v.y;
		m[14] = v.z;
		return *this;
	}

	mat4& rotate(float angleDegree, const vec3& axis)
	{
		float rad = DegtoRad(angleDegree);
		float cos = std::cos(rad);
		float sin = std::sin(rad);

		mat4 rotation;

		if (axis.x != 0.0f) {
			rotation[5] = cos;
			rotation[6] = sin;
			rotation[9] = -sin;
			rotation[10] = cos;
		}
		else if (axis.y != 0.0f) {
			rotation[0] = cos;
			rotation[2] = -sin;
			rotation[8] = sin;
			rotation[10] = cos;

		}
		else if (axis.z != 0.0f) {
			rotation[0] = cos;
			rotation[1] = sin;
			rotation[4] = -sin;
			rotation[5] = cos;
		}
		*this = *this * rotation;
		return *this;
	}

	mat4& rotate(const vec3& anglesDegree)
	{
		if (anglesDegree.x != 0.0f)
			rotate(anglesDegree.x, { 1.0f, 0.0f, 0.0f });
		if (anglesDegree.y != 0.0f)
			rotate(anglesDegree.y, { 0.0f, 1.0f, 0.0f });
		if (anglesDegree.z != 0.0f)
			rotate(anglesDegree.z, { 0.0f, 0.0f, 1.0f });
		return *this;
	}

	mat4& scale(const vec3& vector)
	{
		m[0] = vector.x;
		m[5] = vector.y;
		m[10] = vector.z;
		return *this;
	}

		// Column - Major(по столбцам)
		// [ m[0]  m[4]  m[8]   m[12]
		// [ m[1]  m[5]  m[9]   m[13]
		// [ m[2]  m[6]  m[10]  m[14]
		// [ m[3]  m[7]  m[11]  m[15]
	mat4& perspective(float fovDegrees, float aspectRatio, float zNear, float zFar)
	{
		for (int i = 0; i < 16; ++i) m[i] = 0.0f;

		float tanHalfFOV = std::tan(DegtoRad(fovDegrees / 2.0f)); // tg(Alpha/2)

		m[0] = 1.0f / (aspectRatio * tanHalfFOV);
		m[5] = 1.0f / tanHalfFOV;

		// Z-mapping в NDC [-1, 1]
		m[10] = -(zFar + zNear) / (zFar - zNear);

		// Смещение по Z 
		m[14] = -(2.0f * zFar * zNear) / (zFar - zNear);

		// Запись -Z в W-компоненту для перспективного деления
		m[11] = -1.0f;
		
		return *this;
	}

	
	mat4& cameraRotate(const vec3& target, const vec3& up)
	{
		vec3 n = (-target).normalized(); // forward, n vector must targeting to positive z axis

		vec3 u = up.cross(n).normalized(); // right

		vec3 v = n.cross(u); // up
	
		m[0] = u.x;
		m[1] = v.x;
		m[2] = n.x;

		m[4] = u.y;
		m[5] = v.y;
		m[6] = n.y;

		m[8] = u.z;
		m[9] = v.z;
		m[10] = n.z;

		return *this;
	}

	mat4& InitCameraTransform(const vec3 position, const vec3& target, const vec3& up)
	{
		mat4 CameraTranslation;
		CameraTranslation.translate({ -position.x, -position.y, -position.z });

		mat4 CameraRotate;
		CameraRotate.cameraRotate(target, up);

		return *this =  CameraRotate * CameraTranslation;
	}

private:
	void identity()
	{
		for (int i = 0; i < 16; i++) {
			m[i] = 0.0f;
		}
		m[0] = m[5] = m[10] = m[15] = 1.0f;
	}
}; 


struct Quat
{
	float w = 0.0f;
	vec3 V;

	static Quat fromAngleAxis(float angle, const vec3& axis) // axis of rotation
	{
		vec3 normal = axis.normalized();
		float halfAngleRad = DegtoRad(angle / 2.0f);
		
		return Quat(cos(halfAngleRad),
			vec3(sin(halfAngleRad) * normal.x,
				 sin(halfAngleRad) * normal.y,
				 sin(halfAngleRad) * normal.z
			)
		);

	}

	Quat(float w, const vec3& V) 
		: w(w), V(V) {}


	Quat Conjugate() const
	{
		return Quat(w, vec3(-V.x, -V.y, -V.z));
	}

	Quat operator* (const vec3& vector) const
	{
		Quat VecQuat(0.0f, vector);

		return *this * VecQuat;

	}

	Quat operator* (const Quat& quat) const
	{
		return Quat((w * quat.w) - (V.x * quat.V.x) - (V.y * quat.V.y) - (V.z * quat.V.z),  // w
			vec3((w * quat.V.x) + (quat.w * V.x) + (V.y * quat.V.z) - (V.z * quat.V.y),    // V.x
				 (w * quat.V.y) + (quat.w * V.y) + (V.z * quat.V.x) - (V.x * quat.V.z),   // V.y
				 (w * quat.V.z) + (quat.w * V.z) + (V.x * quat.V.y) - (V.y * quat.V.x))  // V.z
		);
	}

};





