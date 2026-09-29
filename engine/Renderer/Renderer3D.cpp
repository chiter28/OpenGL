#include "Renderer3D.h"

#include <glad/glad.h>

#include "Models/Model.h"
#include "Scene/Camera.h"
#include "Resources/Shader.h"
#include "Materials/Material.h"



void Renderer3D::Init()
{
	s_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");
	s_MaterialBinder = std::make_unique<MaterialBinder>();

	s_Shader->Bind();
	s_MaterialBinder->ConfigureShader(*s_Shader);
	s_Shader->Unbind();
}

void Renderer3D::Shutdown()
{
	s_DrawQueue.clear();
	s_MaterialBinder.reset();

	if (s_Shader)
		s_Shader->Unbind();
	s_Shader.reset();
}

void Renderer3D::BeginScene(const Camera& camera, const DirectionalLight& light)
{
	s_DrawQueue.clear();

	s_SceneData.ViewMatrix = camera.GetView();
	s_SceneData.ProjectionMatrix = camera.GetPerspectiveProjection();
	s_SceneData.Light = light;
}

void Renderer3D::EndScene()
{

	const bool isDataView =
		s_DebugView == DebugView::Normals ||
		s_DebugView == DebugView::Metallic ||
		s_DebugView == DebugView::Roughness;

	if (isDataView)
	{
		glDisable(GL_FRAMEBUFFER_SRGB);
	}
	else
	{
		glEnable(GL_FRAMEBUFFER_SRGB);
	}

	// Pass 1: Opaque (Непрозрачные сабмеши)
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	RenderPass(false);

	
	// Pass 2: Transparent (Прозрачные сабмеши)
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderPass(true);


	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_FRAMEBUFFER_SRGB);

}


void Renderer3D::DrawModel(const std::shared_ptr<Model>& model, const glm::mat4& transform)
{
	if (model)
		s_DrawQueue.emplace_back(model, transform);
}

void Renderer3D::RenderPass(bool transparentPass)
{
	Shader* lastBoundShader = nullptr;

	for (const auto& command : s_DrawQueue)
	{
		const auto& model = command.ModelAsset;

		for (const auto& instance : model->GetInstances())
		{
			const Mesh& mesh = model->GetMesh(instance.MeshIndex);
			
			const glm::mat4 meshToWorld = command.Transform * instance.Transform;

			mesh.GetVertexArray().Bind();

			for (const auto& subMesh : mesh.GetSubMeshes())
			{
				const auto& material = model->GetMaterial(subMesh.MaterialIndex);
				if (!material || material->Transparent != transparentPass)
					continue;

				// MVP, Light
				if (s_Shader.get() != lastBoundShader)
				{
					s_Shader->Bind();

					s_Shader->SetInt("u_DebugView", static_cast<int>(s_DebugView));

					s_Shader->SetMat4("u_View", s_SceneData.ViewMatrix);
					s_Shader->SetMat4("u_Projection", s_SceneData.ProjectionMatrix);
					
					s_SceneData.Light.CalculateViewDir(s_SceneData.ViewMatrix);
					s_SceneData.Light.Bind(*s_Shader);

					lastBoundShader = s_Shader.get();
				}

				s_Shader->SetMat4("u_Model", meshToWorld);

				// Normals matrix
				const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm:: mat3(s_SceneData.ViewMatrix * meshToWorld)));
				s_Shader->SetMat3("u_NormalMatrix", normalMatrix);
				


				// Material
				s_MaterialBinder->Apply(*material);


				// Draw
				glDrawElementsBaseVertex(GL_TRIANGLES,
					subMesh.IndexCount,
					GL_UNSIGNED_INT,
					(const void*)(uintptr_t)(subMesh.IndexOffset * sizeof(uint32_t)),
					subMesh.VertexOffset
				);
			}
		}
	}
}