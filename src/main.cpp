#include <glad/glad.h> // Before glfw3.h
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <array>

#include "Utils.h"
#include <fstream>

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

#include "Renderer/WorldTransformation.h"

#include "Renderer/WindowManager.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



int windowWidth = 1920;
int windowHeight = 1080;
CameraHz cameraH;

WorldTransformation CubeWorldTransform;
Camera camera(windowWidth, windowHeight, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));



void set_new_framebuffer_size(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;

    cameraH.SetViewportSize(width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


  

int main()
{


    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
    
    WindowManager windowManager(windowWidth, windowHeight, "App");

    
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(windowManager.GetNativeWindow(), set_new_framebuffer_size);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    //glEnable(GL_DEPTH_TEST);

    // OpenGL
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    
    // create Vertex(position and color)
    std::vector<Vertex> vertices;
    vertices.resize(8);
    vertices[0] = Vertex( 0.5f,  0.5f,  0.5f,      1.0f, 0.2f, 0.8f);
    vertices[1] = Vertex( 0.5f,  0.5f, -0.5f,      0.5f, 1.0f, 0.5f);
    vertices[2] = Vertex(-0.5f,  0.5f,  0.5f,      1.0f, 0.2f, 0.8f);
    vertices[3] = Vertex(-0.5f,  0.5f, -0.5f,      1.0f, 0.2f, 0.8f);
    vertices[4] = Vertex( 0.5f, -0.5f,  0.5f,      0.2f, 0.3f, 0.0f);
    vertices[5] = Vertex( 0.5f, -0.5f, -0.5f,      0.2f, 0.4f, 0.1f);
    vertices[6] = Vertex(-0.5f, -0.5f,  0.5f,      1.0f, 0.2f, 0.8f);
    vertices[7] = Vertex(-0.5f, -0.5f, -0.5f,      0.6f, 0.4f, 0.0f);
  

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


    

    glClearColor(0.0, 0.3f, 0.3f, 1.0f);

    Shader shader("Resources/shaders/shader.glsl");

    shader.Bind();


    while (!glfwWindowShouldClose(windowManager.GetNativeWindow())) {

        glClear(GL_COLOR_BUFFER_BIT); //| GL_DEPTH_BUFFER_BIT);
        processInput(windowManager.GetNativeWindow());
        

        vertexArray->Bind();
     
        {
            glm::mat4 World = glm::mat4(1.0f);
            World = glm::translate(World, glm::vec3(0.0f, 0.0f, -3.0f));
            World = glm::rotate(World, 0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
            World = glm::scale(World, glm::vec3(1.0f, 1.0f, 1.0f));


            cameraH.OnUpdate();
            const glm::mat4 View = cameraH.GetView();

            glm::mat4 FinalTransform = cameraH.GetPerspectiveProjection() * View * World;

            shader.SetMat4("uTranslation", FinalTransform);
        }


        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);


        glfwSwapBuffers(windowManager.GetNativeWindow());
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}