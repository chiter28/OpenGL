#include "Slider.h"

#include <algorithm>


#include "UIRenderer.h"




void Slider::Update(glm::vec2 mousePos, bool leftMouseDown, float& value)
{
	// slider
	bool hovered = false;

	if (mousePos.x >= sliderX
		&& mousePos.y >= sliderY
		&& mousePos.x <= (sliderX + sliderWidth)
		&& mousePos.y <= (sliderY + sliderHeight)
		)
	{
		hovered = true;
	}

	const bool leftMousePressed = leftMouseDown && !m_PrevLeftMouseDown;

	const float thumbLeft = sliderX + value * (sliderWidth - thumbWidth);

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
		float normalizedValue = (desiredLeft - sliderX) / (sliderWidth - thumbWidth);
		normalizedValue = std::clamp(normalizedValue, 0.0f, 1.0f);

		value = normalizedValue;
	}

	m_PrevLeftMouseDown = leftMouseDown;
}





void Slider::Draw(UiRenderer& renderer, float value) const
{
		renderer.DrawRect({ sliderX, sliderY }, { sliderWidth, sliderHeight }, { 1.0f, 0.0f, 0.0f, 1.0f });

		const float thumbLeft = sliderX + value * (sliderWidth - thumbWidth);
		renderer.DrawRect({ thumbLeft, sliderY }, { thumbWidth, sliderHeight }, { 1.0f, 1.0f, 1.0f, 1.0f });
}
