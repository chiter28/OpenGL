#pragma once

#include <memory>


#include <glm/glm.hpp>
#include "Renderer/Resources/Shader.h"
#include "Renderer/Resources/VertexArray.h"



class UiRenderer
{
public:
	UiRenderer();

	void Begin(float windowWidth, float windowHeight);
	void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color);
	void End();
	
private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<VertexArray> m_VertexArray;

	bool m_DepthTestWasEnable = false;
	bool m_CullFaceWasEnable = false;
	bool m_BlendWasEnable = false;

};