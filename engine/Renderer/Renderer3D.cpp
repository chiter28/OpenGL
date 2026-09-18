#include "Renderer3D.h"

#include <glad/glad.h>

#include "Models/Model.h"
#include "Scene/Camera.h"
#include "Resources/Shader.h"
#include "Materials/Material.h"



void Renderer3D::Init()
{
	s_Shader = std::make_shared<Shader>("Resources/shaders/shader.glsl");


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

		for (const auto& mesh : model->GetMeshes())
		{
			mesh.GetVertexArray().Bind();

			for (const auto& subMesh : mesh.GetSubMeshes())
			{
				const auto& material = model->GetMaterial(subMesh.MaterialIndex);
				if (!material || material->Transparent != transparentPass)
					continue;

				s_SceneData.Light.CalculateViewDir(s_SceneData.ViewMatrix);


				if (s_Shader.get() != lastBoundShader)
				{
					s_Shader->Bind();

					s_Shader->SetMat4("u_View", s_SceneData.ViewMatrix);
					s_Shader->SetMat4("u_Projection", s_SceneData.ProjectionMatrix);
					s_SceneData.Light.Bind(*s_Shader);
					lastBoundShader = s_Shader.get();
				}

				s_Shader->SetMat4("u_Model", command.Transform);


				s_Shader->SetVec4("u_Material.BaseColorFactor", material->BaseColor);
				s_Shader->SetVec3("u_Material.SpecularColor", material->SpecularColor);
				s_Shader->SetFloat("u_Material.Shininess", material->Shininess);



				s_Shader->SetInt("u_Has_AlbedoMap", material->BaseColorTexture ? 1 : 0);
				if (material->BaseColorTexture)
				{
					material->BaseColorTexture->Bind(0);
					s_Shader->SetInt("u_AlbedoMap", 0);
				}



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

/*

#include "Renderer3D.h"

#include <glad/glad.h>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

void Renderer3D::BeginScene(const Camera& camera, const DirectionalLight& light)
{
	s_DrawQueue.clear();

	s_SceneData.ViewMatrix = camera.GetView();
	s_SceneData.ProjectionMatrix = camera.GetPerspectiveProjection();
	s_SceneData.Light = light;
}

void Renderer3D::EndScene()
{
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
}


void Renderer3D::DrawMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)
{
	if (mesh)
		s_DrawQueue.emplace_back(mesh, transform);
}

void Renderer3D::RenderPass(bool transparentPass)
{
	Shader* lastBoundShader = nullptr;

	for (const auto& command : s_DrawQueue)
	{
		const auto& mesh = command.MeshAsset;
		const auto& vertexArray = mesh->GetVertexArray();
		const auto& subMeshes = mesh->GetSubMeshes();

		vertexArray->Bind();

		for (auto& subMesh : subMeshes) {

			const auto& material = mesh->GetMaterial(subMesh.MaterialIndex);
			if (!material || material->IsTransparent() != transparentPass)
				continue;

			material->Bind();

			s_SceneData.Light.CalculateViewDir(s_SceneData.ViewMatrix);

			std::shared_ptr<Shader>& shader = material->GetShader();
			if (shader.get() != lastBoundShader)
			{
				shader->SetMat4("u_View", s_SceneData.ViewMatrix);
				shader->SetMat4("u_Projection", s_SceneData.ProjectionMatrix);
				s_SceneData.Light.Bind(*shader);

				lastBoundShader = shader.get();
			}

			shader->SetMat4("u_Model", command.Transform);


			glDrawElementsBaseVertex(GL_TRIANGLES,
				subMesh.NumIndices,
				GL_UNSIGNED_INT,
				(const void*)(uintptr_t)(subMesh.BaseIndex * sizeof(uint32_t)),
				subMesh.BaseVertex
			);
		}
	}
}



*/