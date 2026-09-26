#pragma once

#include <memory>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>

#include "Core/Sandbox.h"
#include "Core/Input.h"

#include "Renderer/Scene/Light.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/Models/Model.h"
#include "Renderer/Materials/Material.h"
#include "Renderer/Geometry/Primitives.h"






class Cube : public Sandbox
{
public:
	Cube() = default;
	~Cube() = default;

	void OnAttach() override
	{
		
		ModelData data;

		// mesh
		MeshData meshData = Primitives::CreateSphereMesh(1.0f, 7, 64);
		data.Meshes.emplace_back(std::move(meshData));

		// Metallic Roughness Texture
		const uint32_t metallicRoughnessTextureIndex = static_cast<uint32_t>(data.Textures.size());
		data.Textures.emplace_back(
			TextureData
			{
				.Data =
					RawTextureData
					{
						.Pixels = {	0, 255, 255, 255 },
						.Width = 1,
						.Height = 1
					},
				.ColorSpace = TextureColorSpace::Linear
			}
		);




		// material
		data.Materials.emplace_back(
			MaterialData
			{
				.BaseColor = glm::vec4(1.0f),
				.Metallic = 1.0f,
				.Roughness = 0.1f,
				.MetallicRoughnessTextureIndex = metallicRoughnessTextureIndex
			}
		);

		m_Model = std::make_shared<Model>(std::move(data));
		
		m_Light = std::make_shared<DirectionalLight>();
	}

	void OnRender(Camera& camera) override
	{
		static float i = 0.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, i, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		i += 0.0002f;

		SetLightDirection();
	
		
		Renderer3D::BeginScene(camera, *m_Light);
		Renderer3D::DrawModel(m_Model, model);
		Renderer3D::EndScene();
	}

	void SetLightDirection() override
	{
		const auto& material = m_Model->GetMaterial(0);
		float& roughness = material->Roughness;
		if (Input::IsKeyPressed(GLFW_KEY_O)) roughness += 0.01f;
		if (Input::IsKeyPressed(GLFW_KEY_L)) roughness -= 0.01f;
		roughness = std::clamp(roughness, 0.0f, 1.0f);


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