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
#include <vector>


App::App()
{ 
	s_Instance = this;
	m_Window = std::make_unique<Window>(1280, 720, "App", m_EventQueue);
	m_Camera = std::make_unique<Camera>();
}




void App::Run()
{
		glEnable(GL_DEPTH_TEST);

	// OpenGL
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	


	using VertexFull = Vertex<Position, Color, TexCoord>; // Pos -> Color -> UV
	
	
	std::vector<Vertex<Position, Color, TexCoord>> cube
	{
		{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },

		{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },
		
		{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },
		
		{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },
		
		{ Position{{-0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{ 0.5f,  0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{ 0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{-0.5f,  0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} },
		
		{ Position{{-0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 0.0f}} },
		{ Position{{ 0.5f, -0.5f, -0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 0.0f}} },
		{ Position{{ 0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{1.0f, 1.0f}} },
		{ Position{{-0.5f, -0.5f,  0.5f}}, Color{{1.0f, 1.0f, 1.0f}}, TexCoord{{0.0f, 1.0f}} }
	};

	std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(cube);

	

	// Index Buffer
	std::array<uint32_t, 36> IndicesCube = {
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
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(IndicesCube);

	// Vertex Array Cube
	std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>(vertexBuffer, indexBuffer);

	
	
	
	

	std::vector<Vertex<Instance>> instanceOffset(1000);
	for (int i = 0; i < 1000; i++) {
		instanceOffset[i].value = glm::vec3(rand() % 50, rand() % 50, rand() % 50);
	}

	auto instanceVB = VertexBuffer::Create(instanceOffset);
	instanceVB->SetLayout({ VertexAttribute::Position });
	// 3. Собираем всё в VertexArray
	auto va = std::make_shared<VertexArray>();

	// Обычный буфер геометрии (divisor = 0 по умолчанию)
	va->AddVertexBuffer(vertexBuffer);
	va->AddVertexBuffer(instanceVB, 1);
	va->AddIndexBuffer(indexBuffer);



	Texture texture("Resources/textures/guc.png");
	Shader shader("Resources/shaders/shader.glsl");


	shader.Bind();
	texture.Bind(0);
	shader.SetInt("uSampler", 0);

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



		
			GLint currentVAO;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);





			vertexArray->Bind();
			for (int i = 0; i < 20; i++) {
				{
					glm::mat4 World = glm::mat4(1.0f);
					World = glm::translate(World, glm::vec3(i +0.5f, 0.0f, 0.0f));
					World = glm::rotate(World, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
					World = glm::scale(World, glm::vec3(1.0f, 1.0f, 1.0f));


					m_Camera->OnUpdate();
					const glm::mat4 View = m_Camera->GetView();

					glm::mat4 FinalTransform = m_Camera->GetPerspectiveProjection() * View * World;

					shader.SetMat4("uMVP", FinalTransform);
				}
				glDrawElements(GL_TRIANGLES, IndicesCube.size(), GL_UNSIGNED_INT, nullptr);
			}
			
			va->Bind();
			glDrawElementsInstanced(GL_TRIANGLES, IndicesCube.size(), GL_UNSIGNED_INT, nullptr, 1000);



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