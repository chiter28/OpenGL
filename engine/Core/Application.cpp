#include "Application.h"

#include <iostream>
#include <memory>


#include "Events/EventDispatcher.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"






App::App()
{ 
	s_Instance = this;
	m_Window = std::make_unique<Window>(1280, 720, "App", m_EventQueue);
	m_Camera = std::make_unique<Camera>();
}




void App::Run()
{
	glEnable(GL_DEPTH_TEST);
	
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	

	while (m_Running && !m_Window->ShouldClose())
	{
		m_Window->PollEvents();

		m_EventQueue.Flush(
			[this](Event& event) {
				App::OnEvent(event);
				m_Camera->OnEvent(event);
				m_CurrentSandbox->OnEvent(event);
			}
		);


		if (!m_Minimized) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			m_CurrentSandbox->OnRender(*m_Camera);

			m_Camera->OnUpdate();

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
		[this](WindowClose& e) {
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