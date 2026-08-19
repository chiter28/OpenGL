#include "WindowManager.h"
#include <iostream>










WindowManager::WindowManager(float width, float height, const char* name)
{

    s_Window = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);

    if (!s_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
    }
    glfwSetInputMode(s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(s_Window);


}
