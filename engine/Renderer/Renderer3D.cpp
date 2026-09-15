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

