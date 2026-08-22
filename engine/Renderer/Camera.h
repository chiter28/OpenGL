#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Events/Event.h"


class Camera
{
public:
	Camera();
	Camera(float fov, float aspectRatio, float nearClip, float farClip);

	void OnUpdate();
	void OnEvent(Event& event);

	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetPerspectiveProjection() const { return m_PerspectiveProjection; }


private:
	void SetViewportSize(float width, float height);
	void UpdateView();

	void MouseRotate(const glm::vec2& delta);
	void MousePan(const glm::vec2& delta);
	void MouseZoom(float delta);

	glm::vec3 GetRight() const { return m_Orientation * glm::vec3(1.0f, 0.0f, 0.0f); }
	glm::vec3 GetUp() const { return m_Orientation * glm::vec3(0.0f, 1.0f, 0.0f); }
	/* Поворачивает вектор с помощью кватерниона на углы Pitch и Yaw.
	Результат — единичный вектор направления от камеры к точке фокуса.
	(кватернионное умножение: Q * V * Q^-1) */
	glm::vec3 GetForward() const { return m_Orientation * glm::vec3(0.0f, 0.0f, -1.0f); }

private:
	float m_FOV = 45.0f;
	float m_AspectRatio = 1.778;
	float m_NearClip = 0.1f;
	float m_FarClip = 500.0f;

	float m_Pitch = 0.0f; // up - down
	float m_Yaw = 0.0f;  // left - right

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	float m_Distance = 10.0f;
	glm::quat m_Orientation = { 1.0f, 0.0f, 0.0f, 0.0f };
	

	glm::vec2 m_PrevMousePos = { 0.0f, 0.0f };
	glm::vec2 m_Viewport = { 1920.0f, 1080.0f };
	
	glm::mat4 m_PerspectiveProjection = glm::mat4(1.0f);
	glm::mat4 m_View = glm::mat4(1.0f);

};