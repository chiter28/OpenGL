#include <glad/glad.h> // Before glfw3.h

#include "Window.h"

#include <iostream>
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"








Window::Window(uint32_t  width, uint32_t height, const char* name, EventQueue& eventQueue)
    : m_EventQueue(eventQueue), m_Width(width), m_Height(height)
{

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    m_Window = glfwCreateWindow(m_Width, m_Height, name, nullptr, nullptr);

    if (!m_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return;
    }

    int displayWidth, displayHeight;
    glfwGetFramebufferSize(m_Window, &displayWidth, &displayHeight);
    glViewport(0, 0, displayWidth, displayHeight);
    m_EventQueue.Push<WindowResize>(displayWidth, displayHeight);
    
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable mouse point
    glfwSwapInterval(1);


    glfwSetWindowUserPointer(m_Window, this);

    SetupCallbacks();
}




void Window::SetupCallbacks()
{
    glfwSetFramebufferSizeCallback(m_Window,
        [](GLFWwindow* window, int width, int height) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (self) {
                self->m_EventQueue.Push<WindowResize>((int32_t)width, (int32_t)height);
            }
        });

    glfwSetWindowCloseCallback(m_Window,
        [](GLFWwindow* window) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (self) {
                self->m_EventQueue.Push<WindowClose>();
            }
        });

    glfwSetKeyCallback(m_Window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!self)
                return;
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                self->m_EventQueue.Push<KeyPress>(key, action == GLFW_REPEAT, (uint8_t)mods);
            }
            else if (action == GLFW_RELEASE) {
                self->m_EventQueue.Push<KeyRelease>(key, (uint8_t)mods);
            }
        });

    glfwSetCursorPosCallback(m_Window,
        [](GLFWwindow* window, double xpos, double ypos) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!self) {
                return;
            }
            self->m_EventQueue.Push<MouseMove>((float)xpos, (float)ypos);
        });

    glfwSetScrollCallback(m_Window,
        [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!self) {
                return;
            }
            self->m_EventQueue.Push<MouseScroll>((float)xoffset, (float)yoffset);
        });

    glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* window, int button, int action, int mods) {
            auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (!self) {
                return;
            }
            if (action == GLFW_PRESS) {
                self->m_EventQueue.Push<MousePress>(button, (uint8_t)mods);
            } else {
                self->m_EventQueue.Push<MouseRelease>(button, (uint8_t)mods);
            }
        });

}



void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}



Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}