#include "UIRenderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

UiRenderer::UiRenderer()
{
	m_Shader = std::make_unique<Shader>("Resources/shaders/ui.vert", "Resources/shaders/ui.frag");

	auto vertexBuffer = VertexBuffer::Create<Vertex<Position>>(
		{
			Position{glm::vec3{0.0f, 0.0f, 0.0f}},
			Position{glm::vec3{1.0f, 0.0f, 0.0f}},
			Position{glm::vec3{1.0f, 1.0f, 0.0f}},

			Position{glm::vec3{0.0f, 0.0f, 0.0f}},
			Position{glm::vec3{1.0f, 1.0f, 0.0f}},
			Position{glm::vec3{0.0f, 1.0f, 0.0f}}
		});

	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexArray->AddVertexBuffer(vertexBuffer);

}




void UiRenderer::Begin(float windowWidth, float windowHeight)
{
	m_DepthTestWasEnable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	m_CullFaceWasEnable = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	m_BlendWasEnable = glIsEnabled(GL_BLEND) == GL_TRUE;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);


	const glm::mat4 projection = glm::ortho(0.0f, windowWidth, windowHeight, 0.0f, -1.0f, 1.0f);

	m_Shader->Bind();
	m_Shader->SetMat4("u_Projection", projection);

	m_VertexArray->Bind();
}




void UiRenderer::DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color)
{
	m_Shader->SetVec4("u_Rect", glm::vec4(position, size));
	m_Shader->SetVec4("u_Color", color);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}





void UiRenderer::End()
{
	if (m_DepthTestWasEnable)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (m_CullFaceWasEnable)
	{
		glEnable(GL_CULL_FACE);
	}
	if (m_BlendWasEnable)
	{
		glEnable(GL_BLEND);
	}
}