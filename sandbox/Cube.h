#pragma once

#include <memory>
#include <iostream>
#include <array>
#include <algorithm>


#include <glm/glm.hpp>



#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Demo.h"

#include "Renderer/Scene/Light.h"
#include "Renderer/Renderer3D.h"
#include "Renderer/Models/Model.h"
#include "Renderer/Materials/Material.h"
#include "Renderer/Geometry/Primitives.h"
#include "Renderer/UI/UIRenderer.h"

#include "Renderer/UI/Slider.h"


#include "Renderer/Loader/glTFLoader.h"



class Cube : public Demo
{
public:
	Cube() = default;
	~Cube() = default;

	void OnAttach() override
	{
		ModelData modelData;
		try
		{
			modelData = glTFLoader::LoadScene("Resources/meshes/wine_barrel/wine_barrel_01_4k.gltf");

		}
		catch (const std::exception& e)
		{
			std::cerr << "glTF loading failed: " << e.what() << "\nUsing a fallback cube\n";

			size_t meshIndex = modelData.Meshes.size();
			
			modelData.Meshes.emplace_back(Primitives::CreateCubeMesh(1.0f));
			modelData.Materials.emplace_back(MaterialData{});

			modelData.Instances.emplace_back(
				MeshInstance
				{
					.MeshIndex = meshIndex,
					.Transform = glm::mat4{ 1.0f }
				}
			);
		}

		m_Model = std::make_shared<Model>(std::move(modelData));

		
		m_Light = std::make_shared<DirectionalLight>();
		Renderer3D::SetDebugView(Renderer3D::DebugView::Normals);


		m_UiRenderer = std::make_unique<UiRenderer>();

		m_SliderRoughness = std::make_unique<Slider>(glm::vec2{ 100.0f, 100.0f }, glm::vec2{ 200.0f, 20.0f });
		m_SliderLightIntensity = std::make_unique<Slider>(glm::vec2{ 100.0f, 200.0f }, glm::vec2{ 200.0f, 20.0f }, 0.0f, 15.0f);
		m_SliderExposure = std::make_unique<Slider>(glm::vec2{ 100.0f, 300.0f }, glm::vec2{ 200.0f, 20.0f }, 0.1f, 10.0f);
	}





	void OnUpdate(float deltaTime) override
	{
		const auto& material = m_Model->GetMaterial(0);
		auto [mouseX, mouseY] = Input::GetMousePos();
		bool leftMouseDown = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

		m_SliderRoughness->Update({ mouseX, mouseY }, leftMouseDown, material->Roughness);
		m_SliderLightIntensity->Update({ mouseX, mouseY }, leftMouseDown, m_Light->m_Intensity);
		m_SliderExposure->Update({ mouseX, mouseY }, leftMouseDown, m_Exposure);

		SetLightDirection(deltaTime);
			
		if (Input::IsKeyPressed(GLFW_KEY_1))
		{
			Renderer3D::SetDebugView(Renderer3D::DebugView::Lit);
		}
		if (Input::IsKeyPressed(GLFW_KEY_2))
		{
			Renderer3D::SetDebugView(Renderer3D::DebugView::BaseColor);
		}
		if (Input::IsKeyPressed(GLFW_KEY_3))
		{
			Renderer3D::SetDebugView(Renderer3D::DebugView::Normals);
		}
		if (Input::IsKeyPressed(GLFW_KEY_4))
		{
			Renderer3D::SetDebugView(Renderer3D::DebugView::Metallic);
		}
		if (Input::IsKeyPressed(GLFW_KEY_5))
		{
			Renderer3D::SetDebugView(Renderer3D::DebugView::Roughness);
		}

	}





	void OnRender(Camera& camera) override
	{
		static float i = 0.0f;
		glm::mat4 m_Transform = glm::mat4(1.0f);
		m_Transform = glm::translate(m_Transform, glm::vec3(0.0f, 0.0f, 0.0f));
		m_Transform = glm::rotate(m_Transform, i, glm::vec3(0.0f, 1.0f, 0.0f));
		m_Transform = glm::scale(m_Transform, glm::vec3(1.0f, 1.0f, 1.0f));

		// Draw Model
		Renderer3D::BeginScene(camera, *m_Light, m_Exposure);
		Renderer3D::DrawModel(m_Model, m_Transform);
		Renderer3D::EndScene();


		// Draw Slider
		const Window& window = App::Get().GetWindow();
		if (window.GetWidth() > 0 && window.GetHeight() > 0)
		{
			m_UiRenderer->Begin(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

			m_SliderRoughness->Draw(*m_UiRenderer, m_Model->GetMaterial(0)->Roughness);
			m_SliderLightIntensity->Draw(*m_UiRenderer, m_Light->m_Intensity);
			m_SliderExposure->Draw(*m_UiRenderer, m_Exposure);

			m_UiRenderer->End();
		}
		
	}





	void SetLightDirection(float deltaTime)
	{
		constexpr float lightSpeed = 2.0f;

		if (Input::IsKeyPressed(GLFW_KEY_W)) m_Light->m_WorldDirection.y -= lightSpeed * deltaTime;
		if (Input::IsKeyPressed(GLFW_KEY_S)) m_Light->m_WorldDirection.y += lightSpeed * deltaTime;
		if (Input::IsKeyPressed(GLFW_KEY_D)) m_Light->m_WorldDirection.x -= lightSpeed * deltaTime;
		if (Input::IsKeyPressed(GLFW_KEY_A)) m_Light->m_WorldDirection.x += lightSpeed * deltaTime;
		
		if (glm::length(m_Light->m_WorldDirection) > 0.0f) {
			m_Light->m_WorldDirection = glm::normalize(m_Light->m_WorldDirection);
		}
		
	}
	


private:
	std::shared_ptr<DirectionalLight> m_Light;
	std::shared_ptr<Model> m_Model;
	std::unique_ptr<UiRenderer> m_UiRenderer;

	float m_Exposure;

	std::unique_ptr<Slider> m_SliderRoughness;
	std::unique_ptr<Slider> m_SliderLightIntensity;
	std::unique_ptr<Slider> m_SliderExposure;
};