#pragma once

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

#include <glm/glm.hpp>

#include <memory>
#include <array>

class Cube : public Sandbox
{
public:
	Cube() = default;

	void OnAttach() override
	{


		std::vector<Vertex<Position, Color, TexCoord>> verticesCube
		{
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(verticesCube);



		std::array<uint32_t, 36> indicesCube = {
			// Передняя
			0, 1, 2,   2, 3, 0,
			// Задняя
			4, 5, 6,   6, 7, 4,
			// Правая
			8, 9, 10,  10, 11, 8,
			// Левая
			12, 13, 14, 14, 15, 12,
			// Верхняя
			16, 17, 18, 18, 19, 16,
			// Нижняя
			20, 21, 22, 22, 23, 20
		};

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indicesCube);

		m_VertexArray = std::make_shared<VertexArray>(vertexBuffer, indexBuffer);


		//std::vector<Vertex<Instance>> instanceOffset(1000);
		//for (int i = 0; i < 1000; i++) {
		//	instanceOffset[i].value = glm::vec3(rand() % 50, rand() % 50, rand() % 50);
		//}
		//
		//auto instanceVB = VertexBuffer::Create(instanceOffset);
		//
		//instanceVB->SetLayout({ VertexAttribute::Instance });
		//// 3. Собираем всё в VertexArray
		//auto va = std::make_shared<VertexArray>();
		//
		//// Обычный буфер геометрии (divisor = 0 по умолчанию)
		//va->AddVertexBuffer(vertexBuffer);
		//va->AddVertexBuffer(instanceVB, 1);
		//va->AddIndexBuffer(indexBuffer);










		m_Texture = std::make_shared<Texture>("Resources/textures/guc.png");

		m_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");
		m_Shader->Bind();

		m_Shader->SetInt("uSampler", 0);
		m_Shader->SetVec3("uMaterial.AmbientColor", glm::vec3(1.0f, 1.0f, 1.0f));
		m_Shader->SetVec3("uLight.Color", glm::vec3(1.0f, 1.0f, 1.0f));
		m_Shader->SetFloat("uLight.AmbientIntensity", 1.0f);
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

		if (m_Texture) {
			m_Texture->Bind(0);
		}

		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


	}

private:
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<Texture> m_Texture;
};