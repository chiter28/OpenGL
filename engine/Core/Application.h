#pragma once
#include <memory>
#include "Window.h"
#include "Renderer/Scene/Camera.h"
#include "Events/EventQueue.h"
#include "Core/Demo.h"

class App
{
public:
	App();
	~App();

	void Run();
	static App& Get() { return *s_Instance; }
	const Window& GetWindow() { return *m_Window; }


	template<typename T>
	void SetDemo()
	{
		if (m_CurrentDemo)
			m_CurrentDemo->OnDetach();
		m_CurrentDemo = std::make_unique<T>();
		m_CurrentDemo->OnAttach();
	}

private:
	void OnEvent(Event& event);


private:
	inline static App* s_Instance = nullptr;

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;

	std::unique_ptr<Demo> m_CurrentDemo;

	EventQueue m_EventQueue;

	bool m_Running = true;
	bool m_Minimized = false;
};