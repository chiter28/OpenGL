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




#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void CheckGLMSIMD()
{
    std::cout << "=== GLM SIMD Configuration Check ===" << std::endl;

    // 1. Проверка макросов архитектуры самого GLM
#if GLM_ARCH & GLM_ARCH_AVX2_BIT
    std::cout << "[GLM] AVX2 Instruction Set: ENABLED" << std::endl;
#elif GLM_ARCH & GLM_ARCH_AVX_BIT
    std::cout << "[GLM] AVX Instruction Set: ENABLED" << std::endl;
#elif GLM_ARCH & GLM_ARCH_SSE42_BIT
    std::cout << "[GLM] SSE4.2 Instruction Set: ENABLED" << std::endl;
#elif GLM_ARCH & GLM_ARCH_SSE2_BIT
    std::cout << "[GLM] SSE2 Instruction Set: ENABLED" << std::endl;
#elif GLM_ARCH & GLM_ARCH_NEON_BIT
    std::cout << "[GLM] ARM NEON Instruction Set: ENABLED" << std::endl;
#else
    std::cout << "[GLM] SIMD is DISABLED (Pure C++ fallback)" << std::endl;
#endif

    // 2. Проверка аппаратных макросов компилятора (Compiler-level intrinsics)
    std::cout << "\n=== Compiler Level Intrinsics ===" << std::endl;

#if defined(__AVX2__)
    std::cout << "[Compiler] __AVX2__ is defined" << std::endl;
#endif
#if defined(__AVX__)
    std::cout << "[Compiler] __AVX__ is defined" << std::endl;
#endif
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
    std::cout << "[Compiler] SSE2 is supported" << std::endl;
#endif
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
    std::cout << "[Compiler] ARM NEON is supported" << std::endl;
#endif

    // 3. Проверка размера и выравнивания типа vec4 в памяти
    std::cout << "\n=== Memory & Alignment Info ===" << std::endl;
    std::cout << "sizeof(glm::vec4): " << sizeof(glm::vec4) << " bytes" << std::endl;
    std::cout << "alignof(glm::vec4): " << alignof(glm::vec4) << " bytes ";

    if (alignof(glm::vec4) >= 16) {
        std::cout << "(Correctly aligned for SIMD!)" << std::endl;
    } else {
        std::cout << "(NOT aligned for SIMD - check GLM_FORCE_DEFAULT_ALIGNED_GENTYPES)" << std::endl;
    }
}




GLFWwindow* window;
int windowWidth = 1920;
int windowHeight = 1080;

WorldTransformation CubeWorldTransform;
Camera camera(windowWidth, windowHeight, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));

void update_uniform(Shader& shader)
{
    CubeWorldTransform.SetPosition(vec3(0.0f, 0.0f, -3.0f));
    CubeWorldTransform.Rotate(vec3(0.0f, 0.05f, 0.0f));
    CubeWorldTransform.SetScale(vec3(1.0f, 1.0f, 3.0f));
    mat4 World = CubeWorldTransform.GetMatrix();


    glfwSetKeyCallback(window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            camera.OnKeyboard(key);
        });
    
    glfwSetCursorPosCallback(window,
        [](GLFWwindow* window, double xPos, double yPos) {
            camera.OnMouse((uint32_t)xPos, (uint32_t)yPos);
        });

    // Camera
    
    
    mat4 View = camera.GetMatrix();
   

    // Perspective Projection
    mat4 Projection; 
    float aspectRatio = (float)windowWidth / windowHeight;
    Projection.perspective(40, aspectRatio, 0.1, 15);


    mat4 FinalTransform = Projection * View * World;

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


    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

    glfwSwapInterval(1);

    window = glfwCreateWindow(mode->width, mode->height, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

    Shader Shader("Resources/shaders/shader.glsl");

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