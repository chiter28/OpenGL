#pragma once

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"

#include "Core/Input.h"

#include <glm/glm.hpp>

#include <memory>


class Barrel : public Sandbox
{
public:
	Barrel() = default;
	
	void OnAttach() override
	{
		m_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");
		m_Shader->Bind();


		m_Mesh.LoadMesh("Resources/Meshes/wine_barrel_01_4k.gltf", m_Shader);

		m_Light = std::make_shared<DirectionalLight>();
		m_Shader->SetVec3("u_Material.SpecularColor", glm::vec3(1.0f));
		m_Shader->SetFloat("u_Material.Shininess", 400.0f);
	}

	void OnRender(Camera& camera) override
	{
		static float i = 0.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, i, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		i += 0.01f;

		m_Light->CalculateViewDir(camera.GetView());
		SetLightDirection();


		glm::mat4 modelView = camera.GetView() * model;

		m_Shader->Bind();
		m_Shader->SetMat4("u_Projection", camera.GetPerspectiveProjection());
		m_Shader->SetMat4("u_ModelView", modelView);


		m_Light->Bind(*m_Shader);

		m_Mesh.Render();
	}

	void SetLightDirection() override
	{

		if (Input::IsKeyPressed(GLFW_KEY_W)) m_Light->m_WorldDirection.y -= 0.01f;
		if (Input::IsKeyPressed(GLFW_KEY_S)) m_Light->m_WorldDirection.y += 0.01f;
		if (Input::IsKeyPressed(GLFW_KEY_D)) m_Light->m_WorldDirection.x -= 0.01f;
		if (Input::IsKeyPressed(GLFW_KEY_A)) m_Light->m_WorldDirection.x += 0.01f;

		if (glm::length(m_Light->m_WorldDirection) > 0.0f) {
			m_Light->m_WorldDirection = glm::normalize(m_Light->m_WorldDirection);
		}

	}

private:
	std::shared_ptr<DirectionalLight> m_Light;
	Mesh m_Mesh;
	std::shared_ptr<Shader> m_Shader;
};