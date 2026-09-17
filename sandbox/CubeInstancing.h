//#pragma once
//
//#include "Core/Sandbox.h"
//
//#include "Renderer/Shader.h"
//#include "Renderer/VertexArray.h"
//#include "Renderer/Camera.h"
//#include "Renderer/Light.h"
//
//
//#include "Core/Input.h"
//
//#include <glm/glm.hpp>
//
//#include <memory>
//#include <array>
//
//class CubeInstancing : public Sandbox
//{
//public:
//	CubeInstancing() = default;
//
//	void OnAttach() override
//	{
//		// VertexBuffer
//		std::vector<Vertex<Position, Color, TexCoord, Normal>> vertices
//		{
//			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//
//			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//
//			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//
//			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//
//			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//
//			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
//			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} }
//		};
//
//
//		// IndexBuffer
//		std::array<uint32_t, 36> indices =
//		{
//			// Передняя
//			0, 1, 2,   2, 3, 0,
//			// Задняя
//			4, 5, 6,   6, 7, 4,
//			// Правая
//			8, 9, 10,  10, 11, 8,
//			// Левая
//			12, 13, 14, 14, 15, 12,
//			// Верхняя
//			16, 17, 18, 18, 19, 16,
//			// Нижняя
//			20, 21, 22, 22, 23, 20
//		};
//
//		for (size_t i = 0; i < indices.size(); i += 3)
//		{
//			uint32_t i0 = indices[i];
//			uint32_t i1 = indices[i + 1];
//			uint32_t i2 = indices[i + 2];
//
//			glm::vec3 p0 = vertices[i0].position;
//			glm::vec3 p1 = vertices[i1].position;
//			glm::vec3 p2 = vertices[i2].position;
//
//			glm::vec3 edge1 = p1 - p0;
//			glm::vec3 edge2 = p2 - p0;
//
//			glm::vec3 faceNormal = glm::cross(edge1, edge2);
//
//			vertices[i0].normal += faceNormal;
//			vertices[i1].normal += faceNormal;
//			vertices[i2].normal += faceNormal;
//		}
//
//		for (auto& v : vertices)
//		{
//			if (glm::length(v.normal) > 0.0f) {
//				v.normal = glm::normalize(v.normal);
//			}
//		}
//		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices);
//		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices);
//
//		// Instancing
//		std::vector<Vertex<Instance>> instanceOffset(1000);
//		for (int i = 0; i < 1000; i++) {
//			instanceOffset[i].instance = glm::vec3(rand() % 50, rand() % 50, rand() % 50);
//		}
//		std::shared_ptr<VertexBuffer> instancingBuffer = VertexBuffer::Create(instanceOffset);
//		instancingBuffer->SetLayout({ VertexAttribute::Instance });
//		
//		// VertexArray
//		m_VertexArray = std::make_shared<VertexArray>();
//		m_VertexArray->AddVertexBuffer(vertexBuffer);
//		m_VertexArray->AddVertexBuffer(instancingBuffer, 1);
//		m_VertexArray->AddIndexBuffer(indexBuffer);
//
//		// Texture
//		m_Texture = std::make_shared<Texture>("Resources/textures/guc.png");
//
//		// Shader
//		m_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");
//		m_Shader->Bind();
//		m_Shader->SetInt("u_AlbedoMap", 0);
//
//		m_Shader->SetVec4("u_Material.BaseColorFactor", glm::vec4(1.0f));
//		m_Shader->SetVec3("u_Material.SpecularColor", glm::vec3(1.0f));
//		m_Shader->SetFloat("u_Material.Shininess", 400.0f);
//
//
//		m_Light = std::make_shared<DirectionalLight>();
//	}
//
//	void OnRender(Camera& camera) override
//	{
//		static float i = 0.0f;
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		model = glm::rotate(model, i, glm::vec3(0.0f, 1.0f, 0.0f));
//		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
//		i += 0.02f;
//
//		SetLightDirection();
//
//		m_Light->CalculateViewDir(camera.GetView());
//	
//
//		m_Shader->Bind();
//		m_Shader->SetMat4("u_Model", model);
//		m_Shader->SetMat4("u_View", camera.GetView());
//		m_Shader->SetMat4("u_Projection", camera.GetPerspectiveProjection());
//
//		m_Light->Bind(*m_Shader);
//
//		if (m_Texture) {
//			m_Texture->Bind(0);
//		}
//
//		m_VertexArray->Bind();
//		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, 1000);
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
//private:
//	std::shared_ptr<DirectionalLight> m_Light;
//	std::shared_ptr<VertexArray> m_VertexArray;
//	std::shared_ptr<Shader> m_Shader;
//	std::shared_ptr<Texture> m_Texture;
//};