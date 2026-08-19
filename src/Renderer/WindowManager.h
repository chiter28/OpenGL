#pragma once
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	WindowManager(float width, float height, const char* name);

	static inline GLFWwindow* GetNativeWindow() { return s_Window; }

private:
	inline static GLFWwindow* s_Window = nullptr;

	float m_Width;
	float m_Height;
	const char* m_Name;
};