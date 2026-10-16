#pragma once

#include <glm/glm.hpp>


class UiRenderer;



class Slider
{
public:
	Slider(glm::vec2 position, glm::vec2 size, float min = 0.0f, float max = 1.0f);

	void Update(glm::vec2 mousePos, bool leftMouseDown, float& value);
	void Draw(UiRenderer& renderer, float value) const;


private:
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	
	float m_Min;
	float m_Max;

	bool m_SliderDragging = false;
	bool m_PrevLeftMouseDown = false;

	float m_ThumbGrabOffset = 0.0f;

	static constexpr float thumbWidth = 12.0f;
};