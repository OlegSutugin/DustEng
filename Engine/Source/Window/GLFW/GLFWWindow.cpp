#include "GLFWWindow.h"
#include <GLFW/glfw3.h>
#include "Log/Log.h"

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindow);

using namespace Dust;

GLFWWindow::GLFWWindow(const WindowSettings& settings)
{
    m_window = glfwCreateWindow(settings.width, settings.height, settings.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        DE_LOG(LogGLFWWindow, Error, "Failed to create GLFW window!");
        return;
    }

    glfwSetWindowPos(m_window, settings.xPos, settings.yPos);
}

GLFWWindow::~GLFWWindow()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void GLFWWindow::setTitle(const std::string& title)
{
    if (!m_window) return;

    glfwSetWindowTitle(m_window, title.c_str());
}

bool GLFWWindow::isValid() const
{
    return m_window;
}

bool GLFWWindow::ShouldClose() const
{
    if (!m_window) return true;
    return glfwWindowShouldClose(m_window);
}
