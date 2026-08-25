#pragma once
#include <memory>
#include "Window.h"
#include "Renderer/Camera.h"
#include "Events/EventQueue.h"

class App
{
public:
	App();
	~App() = default;

	void Run();
	static App& Get() { return *s_Instance; }
	const Window& GetWindow() { return *m_Window; }

private:

	void OnEvent(Event& event);

	inline static App* s_Instance = nullptr;

	EventQueue m_EventQueue;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;


	bool m_Running = true;
	bool m_Minimized = false;

};