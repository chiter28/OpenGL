#pragma once

#include <memory>
#include <array>

#include "Core/Sandbox.h"
#include "Core/Input.h"

#include "Renderer/Scene/Light.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/Models/Model.h"





#include <glm/glm.hpp>


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
		TextureData texture;
		texture.Data = FileTextureData
		{ 
			.Path = "Resources/textures/guc.png"
		};


		data.Textures.emplace_back(TextureData {
				.Data = FileTextureData {
					.Path = "Resources/textures/guc.png"
				}
		});

		data.Textures.emplace_back(TextureData{
				.Data = FileTextureData {
					.Path = "Resources/textures/image.JPEG"
				}
		});



		// material
		MaterialData materialData1
		{
			.BaseColor = glm::vec4(1.0f),
			.SpecularColor = glm::vec3(1.0f),
			.Shininess = 400.0f,
			.BaseColorTextureIndex = 0
		};

		MaterialData materialData2
		{
			.BaseColor = glm::vec4(1.0f, 0.3f, 0.5f, 1.0f),
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