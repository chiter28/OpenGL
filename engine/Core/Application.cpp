#include "Application.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

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
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);


	std::vector<Vertex> vertices = {
		// Позиция (X, Y, Z)            Цвет (R, G, B)      UV (U, V)

		// === Передняя грань (+Z) ===
		Vertex(-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 0: Низ-Лево
		Vertex(0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 1: Низ-Право
		Vertex(0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 2: Верх-Право
		Vertex(-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 3: Верх-Лево

		// === Задняя грань (-Z) ===
		Vertex(0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 4
		Vertex(-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 5
		Vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 6
		Vertex(0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 7

		// === Правая грань (+X) ===
		Vertex(0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 8
		Vertex(0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 9
		Vertex(0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 10
		Vertex(0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 11

		// === Левая грань (-X) ===
		Vertex(-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 12
		Vertex(-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 13
		Vertex(-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 14
		Vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 15

		// === Верхняя грань (+Y) ===
		Vertex(-0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 16
		Vertex(0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 17
		Vertex(0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 18
		Vertex(-0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f), // 19

		// === Нижняя грань (-Y) ===
		Vertex(-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f), // 20
		Vertex(0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 0.0f), // 21
		Vertex(0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   1.0f, 1.0f), // 22
		Vertex(-0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f,   0.0f, 1.0f)  // 23
	};





	// Vertex Buffer
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(vertices);
	vertexBuffer->SetLayout({
			VertexAttribute::Position,
			VertexAttribute::Color,
			VertexAttribute::TexCoord
		});


	// Index Buffer
	std::array<uint32_t, 36> Indices = {
		// Передняя
		0, 1, 2,   2, 3, 0,
		// Задняя
		4, 5, 6,   6, 7, 4,
		// Правая
		8, 9, 10,  10, 11, 8,
		// Левая
		12, 13, 14, 14, 15, 12,
		// Верхняя
		16, 17, 18, 18, 19, 16,
		// Нижняя
		20, 21, 22, 22, 23, 20
	};
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(Indices);

	// Vertex Array
	std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(vertexBuffer, indexBuffer);


	Texture texture("Resources/textures/image.JPEG");




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
				World = glm::translate(World, glm::vec3(0.0f, 0.0f, 0.0f));
				World = glm::rotate(World, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				World = glm::scale(World, glm::vec3(1.0f, 1.0f, 1.0f));


				m_Camera->OnUpdate();
				const glm::mat4 View = m_Camera->GetView();

				glm::mat4 FinalTransform = m_Camera->GetPerspectiveProjection() * View * World;

				shader.SetMat4("uMVP", FinalTransform);
			}

			texture.Bind(0);
			shader.SetInt("uSampler", 0);

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