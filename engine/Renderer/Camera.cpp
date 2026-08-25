#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Input.h"

#include "Events/EventDispatcher.h"



Camera::Camera()
	: m_PerspectiveProjection(glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip))
{
	UpdateView();
}

Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip)
	: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_AspectRatio = m_Viewport.x / m_Viewport.y;
	m_PerspectiveProjection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	UpdateView();
}

void Camera::OnUpdate()
{
}

void Camera::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMove>(
		[this](MouseMove& e) {
			glm::vec2 delta = (glm::vec2(e.X, e.Y) - m_PrevMousePos);
			m_PrevMousePos = glm::vec2(e.X, e.Y);

			if (Input::IsKeyPressed(GLFW_KEY_LEFT_ALT))
			{
				if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
					MouseRotate(delta);
					UpdateView();
					return true;
				}

				if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
					MousePan(delta);
					UpdateView();
					return true;
				}

				if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
					MouseZoom(delta.y);
					UpdateView();
					return true;
				}
			}
			return false;
		}
	);

	dispatcher.Dispatch<MouseScroll>(
		[this](MouseScroll& e) {
			if (Input::IsKeyPressed(GLFW_KEY_LEFT_ALT))
			{
				MouseZoom(e.YOffset * 10.0f);
				UpdateView();
				return true;
			}
			return false;
		}
	);

	dispatcher.Dispatch<WindowResize>(
		[this](WindowResize& e) {
			SetViewportSize(e.Width, e.Height);
			return false;
		}
	);
}

void Camera::SetViewportSize(float width, float height)
{
	if (height > 0) {
		m_AspectRatio = width / height;
		m_PerspectiveProjection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}
}

void Camera::UpdateView()
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

void Camera::MouseRotate(const glm::vec2& delta)
{
	float rotationSpeed = 0.0025f;
	m_Yaw += delta.x * rotationSpeed;
	m_Pitch += delta.y * rotationSpeed;

	m_Pitch = glm::clamp(m_Pitch, -glm::radians(89.0f), glm::radians(89.0f));

	// Кватернион ориентации из углов.
	// Хранит в себе информацию о вращении в пространстве (это инструкция о повороте).
	m_Orientation = glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}


void Camera::MousePan(const glm::vec2& delta)
{
	float panSpeed = 0.001f * m_Distance;
	m_FocalPoint += -GetRight() * delta.x * panSpeed;
	m_FocalPoint += GetUp() * delta.y * panSpeed;
}

void Camera::MouseZoom(float delta)
{
	float zoomSpeed = 0.001f * m_Distance;
	m_Distance -= delta * zoomSpeed;
}
