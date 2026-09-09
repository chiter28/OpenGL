#pragma once

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/Mesh.h"
#include "Renderer/Camera.h"

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

		m_Shader->SetVec3("uLight.Color", glm::vec3(1.0f, 1.0f, 1.0f));
		m_Shader->SetFloat("uLight.AmbientIntensity", 1.0f);
		m_Mesh.LoadMesh("Resources/Meshes/wine_barrel_01_4k.gltf", m_Shader);
	}

	void OnRender(Camera& camera) override
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		glm::mat4 mvp = camera.GetPerspectiveProjection() * camera.GetView() * model;

		m_Shader->Bind();
		m_Shader->SetMat4("uMVP", mvp);


		m_Mesh.Render();
	}

private:
	Mesh m_Mesh;
	std::shared_ptr<Shader> m_Shader;
};