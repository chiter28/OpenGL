#include <glad/glad.h> // Before glfw3.h
#include <GLFW/glfw3.h>
#include <iostream>

#include "Utils.h"
#include <fstream>

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"



void update_uniform(Shader& shader)
{
    static float s_scale = 0.0f;
    static float s_delta = 0.002f;

    s_scale += s_delta;
    if (s_scale >= 1.0f || s_scale <= -1.0f)
        s_delta *= -1.0f;

    shader.SetFloat("u_Scale", s_scale);
}



void set_new_framebuffer_size(GLFWwindow* window, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, set_new_framebuffer_size);


    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    Vec3f Vertices[3];
    Vertices[0] = Vec3f(-1.0f, -1.0f, 0.0f);
    Vertices[1] = Vec3f( 0.0f,  1.0f, 0.0f);
    Vertices[2] = Vec3f( 1.0f, -1.0f, 0.0f);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    VertexBuffer VertexBuf(sizeof(Vertices), reinterpret_cast<float*>(Vertices));



    glClear(GL_COLOR_BUFFER_BIT);
    
    VertexBuf.Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,0);

    glClearColor(0.0, 0.3f, 0.3f, 1.0f);

    Shader Shader("../Resources/shaders/shader.glsl");

    Shader.Bind();


    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);
        

        glDrawArrays(GL_TRIANGLES, 0, 3);


        update_uniform(Shader);
      


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDisableVertexAttribArray(0);
    glfwTerminate();
    return 0;
}