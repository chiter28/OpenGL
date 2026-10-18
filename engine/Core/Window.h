#pragma once

#include <functional>
#include "Events/EventQueue.h"

struct GLFWwindow;


class Window
{
public:
	using EventCallbackFn = std::function<void(int32_t, int32_t)>;
	
	Window(uint32_t width, uint32_t height, const char* name, EventQueue& eventQueue);
	~Window();


	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;


	void PollEvents();
	void SwapBuffers();

	bool ShouldClose();


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