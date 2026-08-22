#include "Input.h"
#include "Application.h"

bool Input::IsKeyPressed(int keycode)
{
	int state = glfwGetKey(App::Get().GetWindow().GetNativeWindow(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	int state = glfwGetMouseButton(App::Get().GetWindow().GetNativeWindow(), button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePos()
{
	double xPos, yPos;
	glfwGetCursorPos(App::Get().GetWindow().GetNativeWindow(), &xPos, &yPos);
	return { static_cast<float>(xPos), static_cast<float>(yPos) };
}
