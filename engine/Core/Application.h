#pragma once
#include <memory>
#include "Window.h"
#include "Renderer/Camera.h"
#include "Events/EventQueue.h"
#include "Core/Sandbox.h"

class App
{
public:
	App();
	~App() = default;

	void Run();
	static App& Get() { return *s_Instance; }
	const Window& GetWindow() { return *m_Window; }


	template<typename T>
	void SetSandbox()
	{
		if (m_CurrentSandbox)
			m_CurrentSandbox->OnDetach();
		m_CurrentSandbox = std::make_unique<T>();
		m_CurrentSandbox->OnAttach();
	}

private:
	void OnEvent(Event& event);


private:
	inline static App* s_Instance = nullptr;

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Camera> m_Camera;

	std::unique_ptr<Sandbox> m_CurrentSandbox;

	EventQueue m_EventQueue;

	bool m_Running = true;
	bool m_Minimized = false;
};