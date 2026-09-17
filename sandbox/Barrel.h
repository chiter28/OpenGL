//#pragma once
//
//#include "Core/Sandbox.h"
//
//#include "Renderer/Shader.h"
//#include "Renderer/Mesh.h"
//#include "Renderer/Camera.h"
//#include "Renderer/Light.h"
//
//#include "Renderer/Renderer3D.h"
//
//#include "Core/Input.h"
//
//#include <glm/glm.hpp>
//
//#include <memory>
//
//
//class Barrel : public Sandbox
//{
//public:
//	Barrel() = default;
//	
//	void OnAttach() override
//	{
//		m_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");
//		m_Shader->Bind();
//
//		m_Mesh = std::make_shared<Mesh>();
//		m_Mesh->LoadMesh("Resources/Meshes/wine_barrel_01_4k.gltf", m_Shader);
//
//		m_Light = std::make_shared<DirectionalLight>();
//		
//	}
//
//	void OnRender(Camera& camera) override
//	{
//		glm::mat4 model = glm::mat4(1.0f);
//		{
//			static float i = 0.0f;
//			model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
//			model = glm::rotate(model, i, glm::vec3(0.0f, 1.0f, 0.0f));
//			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
//			i += 0.01f;
//		}
//
//		SetLightDirection();
//
//		Renderer3D::BeginScene(camera, *m_Light);
//		Renderer3D::DrawMesh(m_Mesh, model);
//		Renderer3D::EndScene();
//		
//	}
//
//	void SetLightDirection() override
//	{
//
//		if (Input::IsKeyPressed(GLFW_KEY_W)) m_Light->m_WorldDirection.y -= 0.01f;
//		if (Input::IsKeyPressed(GLFW_KEY_S)) m_Light->m_WorldDirection.y += 0.01f;
//		if (Input::IsKeyPressed(GLFW_KEY_D)) m_Light->m_WorldDirection.x -= 0.01f;
//		if (Input::IsKeyPressed(GLFW_KEY_A)) m_Light->m_WorldDirection.x += 0.01f;
//
//		if (glm::length(m_Light->m_WorldDirection) > 0.0f) {
//			m_Light->m_WorldDirection = glm::normalize(m_Light->m_WorldDirection);
//		}
//
//	}
//
//private:
//	std::shared_ptr<DirectionalLight> m_Light;
//	std::shared_ptr<Mesh> m_Mesh;
//	std::shared_ptr<Shader> m_Shader;
//};