#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


class Camera
{
public:
	Camera() = default;
	Camera(int windowWidth, int windowHeight, const vec3& position, const vec3& target, const vec3& up);
	
	void SetPosition(vec3 position);
	void OnKeyboard(uint32_t key);
	void OnMouse(int32_t x, int32_t y);

	mat4 GetMatrix() const;

private:
	void Init();
	void Update();

private:
	vec3 m_Position = vec3(0.0f, 0.0f, 0.0f);
	vec3 m_Target = vec3(0.0f, 0.0f, -1.0f);
	vec3 m_Up = vec3(0.0f, 1.0f, 0.0f);
	float m_Speed = 1.0f;

	float m_WindowWidth = 1020;
	float m_WindowHeight = 1980;

	float m_Yaw = 0.0f; // horizont
	float m_Pitch = 0.0f; // vertical

	bool m_OnUpperEdge = false;
	bool m_OnLowerEdge = false;
	bool m_OnLeftEdge = false;
	bool m_OnRightEdge = false;

	vec3 m_MousePos = vec3(0.0f, 0.0f, 0.0f);

};

class CameraHz
{
public:
	CameraHz();
	CameraHz(float fov, float aspectRatio, float nearClip, float farClip);

	void OnUpdate();

	const glm::mat4& GetView() const { return m_View; }
	const glm::mat4& GetPerspectiveProjection() const { return m_PerspectiveProjection; }
	void SetViewportSize(float width, float height);


private:
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