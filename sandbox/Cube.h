#pragma once

#include <memory>
#include <array>

#include "Core/Sandbox.h"
#include "Core/Input.h"

#include "Renderer/Scene/Light.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/Models/Model.h"


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


MeshData CreateSphereMesh(float radius, uint32_t sectors, uint32_t stacks)
{
	std::vector<ModelVertex> vertices;
	std::vector<uint32_t> indices;

	float sectorStep = 2.0f * glm::pi<float>() / sectors;
	float stackStep = glm::pi<float>() / stacks;

	// 1. Генерация вершин, нормалей и UV
	for (uint32_t i = 0; i <= stacks; ++i)
	{
		// Угол широты: от +pi/2 (верхний полюс) до -pi/2 (нижний полюс)
		float stackAngle = glm::pi<float>() / 2.0f - i * stackStep;
		float xy = radius * cosf(stackAngle);
		float y = radius * sinf(stackAngle);

		for (uint32_t j = 0; j <= sectors; ++j)
		{
			// Угол долготы: от 0 до 2*pi
			float sectorAngle = j * sectorStep;

			float x = xy * cosf(sectorAngle);
			float z = xy * sinf(sectorAngle);

			// Текстурные координаты UV в диапазоне [0, 1]
			float u = static_cast<float>(j) / sectors;
			float v = static_cast<float>(i) / stacks;

			// Аналитическая нормаль для сферы
			glm::vec3 norm = glm::normalize(glm::vec3(x, y, z));

			vertices.push_back({
				Position{{ x, y, z }},
				Color{{ 1.0f, 1.0f, 1.0f }},
				TexCoord{{ u, v }},
				Normal{{ norm.x, norm.y, norm.z }}
				});
		}
	}



	// 2. Генерация индексов (треугольников)
	for (uint32_t i = 0; i < stacks; ++i)
	{
		uint32_t k1 = i * (sectors + 1);     // Начало текущей широты
		uint32_t k2 = k1 + sectors + 1;      // Начало следующей широты

		for (uint32_t j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			// Верхний треугольник квада (пропускаем на самом верхнем полюсе)
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// Нижний треугольник квада (пропускаем на самом нижнем полюсе)
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	// 3. Формирование SubMesh
	std::vector<SubMesh> subMeshes = {
		{
			.IndexOffset = 0,
			.IndexCount = static_cast<uint32_t>(indices.size()),
			.VertexOffset = 0,
			.MaterialIndex = 0
		}
	};

	return MeshData{
		.Vertices = std::move(vertices),
		.Indices = std::move(indices),
		.SubMeshes = std::move(subMeshes)
	};
}







class Cube : public Sandbox
{
public:
	Cube() = default;

	void OnAttach() override
	{

		std::vector<Vertex<Position, Color, TexCoord, Normal>> vertices
		{
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },

			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },

			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },

			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },

			{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },

			{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} },
			{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}}, Normal{{0.0f, 0.0f, 0.0f}} }
		};

		std::vector<uint32_t> indices = {
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


		// vertices Normals
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			uint32_t i0 = indices[i];
			uint32_t i1 = indices[i + 1];
			uint32_t i2 = indices[i + 2];

			glm::vec3 p0 = vertices[i0].position;
			glm::vec3 p1 = vertices[i1].position;
			glm::vec3 p2 = vertices[i2].position;

			glm::vec3 edge1 = p1 - p0;
			glm::vec3 edge2 = p2 - p0;

			glm::vec3 faceNormal = glm::cross(edge1, edge2);
		
			vertices[i0].normal += faceNormal;
			vertices[i1].normal += faceNormal;
			vertices[i2].normal += faceNormal;
		}

		for (auto& v : vertices)
		{
			if (glm::length(v.normal) > 0.0f) {
				v.normal = glm::normalize(v.normal);
			}
		}















		// init model

		std::vector<SubMesh> subMeshes = {
			 {
				.IndexOffset = 0,
				.IndexCount = 18,
				.VertexOffset = 0,
				.MaterialIndex = 0
			},
			{
				.IndexOffset = 18,
				.IndexCount = 18,
				.VertexOffset = 0,
				.MaterialIndex = 1
			}
		};
		



		ModelData data;

		// mesh
		MeshData meshData = {
			.Vertices = std::move(vertices),
			.Indices = std::move(indices),
			.SubMeshes = std::move(subMeshes)
		};
		data.Meshes.emplace_back(std::move(meshData));


		// texture
		data.Textures.emplace_back(
			TextureData
			{
				.Data =
					FileTextureData
					{
						.Path = "Resources/textures/guc.png"
					},
				.ColorSpace = TextureColorSpace::sRGB
			}
		);

		data.Textures.emplace_back(
			TextureData
			{
				.Data =
					FileTextureData
					{
						.Path = "Resources/textures/image.JPEG"
					},
				.ColorSpace = TextureColorSpace::sRGB
			}
		);

	



		// material
		MaterialData materialData1
		{
			.BaseColor = glm::vec4(1.0f),

			.Metallic = 1.0f,
			.Roughness = 0.2f,

			.SpecularColor = glm::vec3(1.0f),
			.Shininess = 400.0f,
			.BaseColorTextureIndex = 0

		};

		MaterialData materialData2
		{
			.BaseColor = glm::vec4(1.0f),

			.Metallic = 0.0f,
			.Roughness = 0.7f,

			.SpecularColor = glm::vec3(1.0f),
			.Shininess = 40.0f,
			.BaseColorTextureIndex = 1
		};
		

		data.Materials.emplace_back(materialData1);
		data.Materials.emplace_back(materialData2);

		m_Model = std::make_shared<Model>(std::move(data));







		Renderer3D::Init();
	

		m_Light = std::make_shared<DirectionalLight>();
	}

	void OnRender(Camera& camera) override
	{

		static float i = 0.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, i, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		i += 0.002f;

	
		SetLightDirection();
		
		Renderer3D::BeginScene(camera, *m_Light);
		Renderer3D::DrawModel(m_Model, model);
		Renderer3D::EndScene();
		
	}

	void SetLightDirection() override
	{

		if (Input::IsKeyPressed(GLFW_KEY_W)) m_Light->m_WorldDirection.y -= 0.02f;
		if (Input::IsKeyPressed(GLFW_KEY_S)) m_Light->m_WorldDirection.y += 0.02f;
		if (Input::IsKeyPressed(GLFW_KEY_D)) m_Light->m_WorldDirection.x -= 0.02f;
		if (Input::IsKeyPressed(GLFW_KEY_A)) m_Light->m_WorldDirection.x += 0.02f;
		
		if (glm::length(m_Light->m_WorldDirection) > 0.0f) {
			m_Light->m_WorldDirection = glm::normalize(m_Light->m_WorldDirection);
		}
		
	}

private:
	std::shared_ptr<DirectionalLight> m_Light;
	std::shared_ptr<Model> m_Model;
};