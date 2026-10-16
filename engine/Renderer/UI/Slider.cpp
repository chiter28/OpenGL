#include "Slider.h"

#include <algorithm>
#include <stdexcept>


#include "UIRenderer.h"




Slider::Slider(glm::vec2 position, glm::vec2 size, float min, float max)
	: m_Position(position), m_Size(size), m_Min(min), m_Max(max)
{
	if (m_Size.x <= thumbWidth || m_Size.y <= 0)
	{
		throw std::invalid_argument("Slider width must exceed thumb width; height must be positive");
	}

	if (min >= max)
	{
		throw std::invalid_argument("Minimum must be less than maximum");
	}
}



void Slider::Update(glm::vec2 mousePos, bool leftMouseDown, float& value)
{
	// slider
	bool hovered = false;

	if (mousePos.x >= m_Position.x
		&& mousePos.y >= m_Position.y
		&& mousePos.x <= (m_Position.x + m_Size.x)
		&& mousePos.y <= (m_Position.y + m_Size.y)
		)
	{
		hovered = true;
	}

	const bool leftMousePressed = leftMouseDown && !m_PrevLeftMouseDown;

	// value convertion - from 0 to 1
	const float t = std::clamp((value - m_Min) / (m_Max - m_Min), 0.0f, 1.0f);


	const float thumbLeft = m_Position.x + t * (m_Size.x - thumbWidth);

	if (!leftMouseDown)
	{
		m_SliderDragging = false;
	}

	if (leftMousePressed && hovered)
	{
		m_SliderDragging = true;
		if (mousePos.x >= thumbLeft && mousePos.x < thumbLeft + thumbWidth)
		{
			m_ThumbGrabOffset = mousePos.x - thumbLeft;
		} else
		{
			m_ThumbGrabOffset = thumbWidth / 2.0f;
		}
	}


	if (m_SliderDragging)
	{
		const float desiredLeft = mousePos.x - m_ThumbGrabOffset;
		float normalizedValue = (desiredLeft - m_Position.x) / (m_Size.x - thumbWidth);
		normalizedValue = std::clamp(normalizedValue, 0.0f, 1.0f);

		value = m_Min + normalizedValue * (m_Max - m_Min);
	}

	m_PrevLeftMouseDown = leftMouseDown;
}





void Slider::Draw(UiRenderer& renderer, float value) const
{
		renderer.DrawRect({ m_Position.x, m_Position.y }, { m_Size.x, m_Size.y }, { 0.5f, 0.2f, 0.8f, 1.0f });

		const float t = std::clamp((value - m_Min) / (m_Max - m_Min), 0.0f, 1.0f);
		const float thumbLeft = m_Position.x + t * (m_Size.x - thumbWidth);
		renderer.DrawRect({ thumbLeft, m_Position.y }, { thumbWidth, m_Size.y }, { 1.0f, 1.0f, 1.0f, 1.0f });
}
