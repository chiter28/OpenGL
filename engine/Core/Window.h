#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Events/EventQueue.h"

class Window
{
public:
	using EventCallbackFn = std::function<void(int32_t, int32_t)>;
	
	Window(uint32_t width, uint32_t height, const char* name, EventQueue& eventQueue);
	~Window();

	void PollEvents();
	void SwapBuffers();

	bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }


	GLFWwindow* GetNativeWindow() const { return m_Window; }
	uint32_t GetWidth() const { return m_Width; }
	uint32_t GetHeight() const { return m_Height; }

private:
	void SetupCallbacks();

private:
	GLFWwindow* m_Window = nullptr;

	EventQueue& m_EventQueue;

	int32_t m_Width = 0;
	int32_t m_Height = 0;
	
};