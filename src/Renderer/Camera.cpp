#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Input/Input.h"



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



CameraHz::CameraHz()
	: m_PerspectiveProjection(glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip))
{}

CameraHz::CameraHz(float fov, float aspectRatio, float nearClip, float farClip)
	: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_AspectRatio = m_Viewport.x / m_Viewport.y;
	m_PerspectiveProjection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void CameraHz::OnUpdate()
{
	auto [xPos, yPos] = Input::GetMousePos();
	glm::vec2 delta = (glm::vec2(xPos, yPos) - m_PrevMousePos);
	
	if (Input::IsKeyPressed(GLFW_KEY_LEFT_ALT))
	{
		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			MouseRotate(delta);
		}

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
			MousePan(delta);
		}

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			MouseZoom(delta.y);
		}
	}

	m_PrevMousePos = glm::vec2(xPos, yPos);
	UpdateView();
}

void CameraHz::SetViewportSize(float width, float height)
{
	if (height > 0) {
		m_AspectRatio = width / height;
		m_PerspectiveProjection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}
}

void CameraHz::UpdateView()
{
	/* Вычисляет позицию самой камеры в мировых координатах.
	Если отступить от точки фокуса m_FocalPoint
	в противоположную сторону от направления взгляда на расстояние m_Distance,
	то получится точка, где физически находится камера */
	m_Position = m_FocalPoint - GetForward() * m_Distance;

	// Помещает виртуальную камеру в 3D - пространство
	// (нужно для расчетов позиций, источников света, звука от первого лица).
	glm::mat4 cameraWorld = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Orientation);

	// Двигает все объекты сцены так, чтобы для экрана это выглядело как вид из камеры
	m_View = glm::inverse(cameraWorld);
}

void CameraHz::MouseRotate(const glm::vec2& delta)
{
	float rotationSpeed = 0.0025f;
	m_Yaw += delta.x * rotationSpeed;
	m_Pitch += delta.y * rotationSpeed;

	m_Pitch = glm::clamp(m_Pitch, -glm::radians(89.0f), glm::radians(89.0f));

	// Кватернион ориентации из углов.
	// Хранит в себе информацию о вращении в пространстве (это инструкция о повороте).
	m_Orientation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}


void CameraHz::MousePan(const glm::vec2& delta)
{
	float panSpeed = 0.01f;
	m_FocalPoint += -GetRight() * delta.x * panSpeed;
	m_FocalPoint += GetUp() * delta.y * panSpeed;
}

void CameraHz::MouseZoom(float delta)
{
	float zoomSpeed = 0.01;
	m_Distance -= delta * zoomSpeed;
}
