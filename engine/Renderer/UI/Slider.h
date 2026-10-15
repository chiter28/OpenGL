#pragma once

#include <glm/glm.hpp>


class UiRenderer;



class Slider
{
public:

	void Update(glm::vec2 mousePos, bool leftMouseDown, float& value);
	void Draw(UiRenderer& renderer, float value) const;


private:

	bool m_SliderDragging = false;
	bool m_PrevLeftMouseDown = false;

	static constexpr float sliderX = 100.0f;
	static constexpr float sliderY = 100.0f;
	static constexpr float sliderWidth = 200.0f;
	static constexpr float sliderHeight = 24.0f;
	static constexpr float thumbWidth = 12.0f;
	float m_ThumbGrabOffset = 0.0f;
};