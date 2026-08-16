#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>




Camera::Camera(int windowWidth, int windowHeight, const vec3& position, const vec3& target, const vec3& up)
	: m_Position(position), m_Target(target.normalized()), m_Up(up.normalized()),
	  m_WindowWidth(windowWidth), m_WindowHeight(windowHeight)
{
	Init();
}

void Camera::SetPosition(vec3 position)
{
	m_Position = position;
}

void Camera::OnKeyboard(uint32_t key)
{
	switch (key)
	{
		case GLFW_KEY_UP:
			m_Position += (m_Target * m_Speed);
			break;

		case GLFW_KEY_DOWN:
			m_Position -= (m_Target * m_Speed);
			break;


		case GLFW_KEY_RIGHT:
		{
			vec3 right = m_Target.cross(m_Up).normalized();
			m_Position += (right * m_Speed);
			break;
		}

		case GLFW_KEY_LEFT:
		{
			vec3 right = m_Target.cross(m_Up).normalized();
			m_Position -= (right * m_Speed);
			break;
		}

		case GLFW_KEY_E:
			m_Position.y += m_Speed;
			break;

		case GLFW_KEY_Q:
			m_Position.y -= m_Speed;
			break;

		case GLFW_KEY_EQUAL:
			m_Speed += 0.1f;
			break;

		case GLFW_KEY_MINUS:
			m_Speed -= 0.1f;
			if (m_Speed < 0.1f)
				m_Speed = 0.1f;
			break;
	}
}

void Camera::OnMouse(int32_t x, int32_t y)
{
	int dX = x - m_MousePos.x;
	int dY = y - m_MousePos.y;
	
	m_MousePos.x = x;
	m_MousePos.y = y;

	float sensitivity = 0.1f; // Чувствительность мыши
	m_Yaw += (float)dX * sensitivity;
	m_Pitch += (float)dY * sensitivity;

	Update();
}

mat4 Camera::GetMatrix() const
{
	mat4 CameraTransformation;
	return CameraTransformation.InitCameraTransform(m_Position, m_Target, m_Up);
}

void Camera::Init()
{
	// Horizontal (left - right)
	float yaw = atan2(m_Target.z, m_Target.x);
	m_Yaw = RadtoDeg(yaw);


	// Vertical (up - down)
	float pitch = asin(m_Target.y);
	m_Pitch = RadtoDeg(pitch);

	m_MousePos = vec3(m_WindowWidth / 2.0f, m_WindowHeight / 2.0f, 0.0f);
}

void Camera::Update()
{
	vec3 yAxis(0.0f, 1.0f, 0.0f); // up

	vec3 view(1.0f, 0.0f, 0.0f);
	// rotate view by horizontal angle around vertical axis V
	view.rotate(m_Yaw, yAxis);
	view = view.normalized();

	vec3 u = yAxis.cross(view); // right
	u.normalized();
	// rotate view by vertical angle around horizontal axis U
	view.rotate(m_Pitch, u);

	m_Target = view.normalized();
	m_Up = m_Target.cross(u).normalized();
}



CameraHz::CameraHz(float fov, float aspectRatio, float nearClip, float farClip)
	: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_AspectRatio = m_Viewport.x / m_Viewport.y;
	m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}
