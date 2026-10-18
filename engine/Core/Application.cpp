#include "Application.h"

#include <iostream>
#include <memory>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Events/EventDispatcher.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

#include "Renderer/Renderer3D.h"
#include "Renderer/Scene/Camera.h"

#include "Window.h"




App::App()
{ 
	s_Instance = this;
	m_Window = std::make_unique<Window>(1280, 720, "App", m_EventQueue);
	m_Camera = std::make_unique<Camera>();

	Renderer3D::Init();
}

App::~App()
{
	if (m_CurrentDemo)
	{
		m_CurrentDemo->OnDetach();
		m_CurrentDemo.reset();
	}
	Renderer3D::Shutdown();
}




void App::Run()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	
	std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();

	while (m_Running && !m_Window->ShouldClose())
	{
		// deltaTime
		const std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
		previousTime = currentTime;


		m_Window->PollEvents();

		m_EventQueue.Flush(
			[this](Event& event) {
				App::OnEvent(event);
				m_Camera->OnEvent(event);
				m_CurrentDemo->OnEvent(event);
			}
		);


		if (!m_Minimized)
		{
			m_Camera->OnUpdate();
			m_CurrentDemo->OnUpdate(deltaTime);
			
			glClearColor(0.01f, 0.02f, 0.03f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			m_CurrentDemo->OnRender(*m_Camera);

			m_Window->SwapBuffers();
		}

	}
}



void App::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<WindowResize>(
		[this](WindowResize& e) {

			if (e.Width == 0 || e.Height == 0) {
				m_Minimized = true;
				return false;
			}

			m_Minimized = false;
			glViewport(0, 0, e.Width, e.Height);
			std::cout << "Resized to: " << e.Width<< "x" << e.Height << std::endl;
			return false;
		}
	);

	dispatcher.Dispatch<WindowClose>(
		[this](WindowClose&) {
			m_Running = false;
			return true;
		}
	);

	dispatcher.Dispatch<KeyPress>(
		[this](KeyPress& e) {
			if (e.KeyCode == GLFW_KEY_ESCAPE) {
				m_Running = false;
			}
			return true;
		}
	);
}