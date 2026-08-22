#include "Application.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

#include "Events/EventDispatcher.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <array>

App::App()
{ 
	s_Instance = this;
	m_Window = std::make_unique<Window>(1280, 720, "App", m_EventQueue);
	m_Camera = std::make_unique<Camera>();
}

void App::Run()
{
	int textureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
	std::cout << textureUnits << std::endl;
	glEnable(GL_DEPTH_TEST);

	// OpenGL
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);


	// create Vertex(position and color)
	std::vector<Vertex> vertices;
	vertices.resize(8);
	vertices[0] = Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.8f);
	vertices[1] = Vertex(0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f);
	vertices[2] = Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.2f, 0.8f);
	vertices[3] = Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 0.2f, 0.8f);
	vertices[4] = Vertex(0.5f, -0.5f, 0.5f, 0.2f, 0.3f, 0.0f);
	vertices[5] = Vertex(0.5f, -0.5f, -0.5f, 0.2f, 0.4f, 0.1f);
	vertices[6] = Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.2f, 0.8f);
	vertices[7] = Vertex(-0.5f, -0.5f, -0.5f, 0.6f, 0.4f, 0.0f);


	// Vertex Array
	std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();

	// Vertex Buffer
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices);
	vertexBuffer->SetLayout({
			VertexElement::Position,
			VertexElement::Color
		});
	vertexArray->AddVertexBuffer(vertexBuffer);

	std::array<uint32_t, 36> Indices =
	{
		// По часовой
		// Передняя (+Z)
		0, 4, 6,   6, 2, 0,

		// Задняя (-Z)
		1, 3, 7,   7, 5, 1,

		// Верхняя (+Y)
		0, 2, 3,   3, 1, 0,

		// Нижняя (-Y)
		4, 5, 7,   7, 6, 4,

		// Правая (+X)
		0, 1, 5,   5, 4, 0,

		// Левая (-X)
		2, 6, 7,   7, 3, 2

	};

	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(Indices);
	vertexArray->AddIndexBuffer(indexBuffer);





	Shader shader("Resources/shaders/shader.glsl");
	shader.Bind();


	while (m_Running && !m_Window->ShouldClose())
	{
		m_Window->PollEvents();

		m_EventQueue.Flush(
			[this](Event& event) {
				OnEvent(event);
				m_Camera->OnEvent(event);
			}
		);


		if (!m_Minimized) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			vertexArray->Bind();

			{
				glm::mat4 World = glm::mat4(1.0f);
				World = glm::translate(World, glm::vec3(0.0f, 0.0f, -3.0f));
				World = glm::rotate(World, 0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
				World = glm::scale(World, glm::vec3(1.0f, 1.0f, 1.0f));


				m_Camera->OnUpdate();
				const glm::mat4 View = m_Camera->GetView();

				glm::mat4 FinalTransform = m_Camera->GetPerspectiveProjection() * View * World;

				shader.SetMat4("uTranslation", FinalTransform);
			}


			glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);



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