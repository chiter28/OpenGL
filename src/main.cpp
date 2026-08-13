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


int windowWidth = 1500;
int windowHeight = 750;

void update_uniform(Shader& shader)
{
    static float scale = 1.0f;
    
    mat4 Scaling;
    Scaling = Scaling.scale({ scale, scale, scale });


    mat4 Rotation;
    static float angle = 0.0f;
    static float rot = 0.0002f;
    angle += rot;
    if (angle >= 360.0f)
        angle = 0.0f;
    Rotation = Rotation.rotate(angle, { 0,1,0 });

    mat4 Translation(0.0, 0.0f, -2.50f);

    mat4 World = Translation * Rotation * Scaling;

    // Camera
    vec3 CameraPos(1.0f, 1.0f, 0.0f);
    vec3 U(1.0f, 0.0f, 0.0f);
    vec3 V(0.0f, 1.0f, 0.0f);
    vec3 N(0.0f, 0.0f, 1.0f);
    mat4 Camera = mat4::camera(CameraPos, U, V, N);


    // Perspective Projection
    float aspectRatio = (float)windowWidth / windowHeight;
    mat4 Projection = mat4::perspective(90, aspectRatio, 0.1, 5);


    mat4 FinalTransform = Projection * Camera * World;

    shader.SetMat4("uTranslation", FinalTransform);
}



void set_new_framebuffer_size(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


  

int main()
{
    // create window
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, set_new_framebuffer_size);
    
    
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

    Shader Shader("../Resources/shaders/shader.glsl");

    Shader.Bind();

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT); //| GL_DEPTH_BUFFER_BIT);
        processInput(window);
        

        vertexArray->Bind();
        update_uniform(Shader);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);


      


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}