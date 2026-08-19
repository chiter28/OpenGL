#include "Input.h"
#include "Renderer/WindowManager.h"

bool Input::IsKeyPressed(int keycode)
{
	int state = glfwGetKey(WindowManager::GetNativeWindow(), keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	int state = glfwGetMouseButton(WindowManager::GetNativeWindow(), button);
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePos()
{
	double xPos, yPos;
	glfwGetCursorPos(WindowManager::GetNativeWindow(), &xPos, &yPos);
	return { static_cast<float>(xPos), static_cast<float>(yPos) };
}
