#include "GLFWWindowManager.h"
#include <GLFW/glfw3.h>
#include "Log/Log.h"
#include "GLFWWindow.h"

DEFINE_LOG_CATEGORY_STATIC(LogGLFWWindowManager);

using namespace Dust;

GLFWWindowManager::GLFWWindowManager()
{
    glfwSetErrorCallback([](int errorCode, const char* description)  //
        { DE_LOG(LogGLFWWindowManager, Error, "GLFW error, code: {}, description: {}", errorCode, description); });

    if (!glfwInit())
    {
        DE_LOG(LogGLFWWindowManager, Error, "Failed to initialize GLFW library!");
        return;
    }

    m_initialized = true;
    DE_LOG(LogGLFWWindowManager, Display, "GLFW library initialized successfully...");
}

GLFWWindowManager::~GLFWWindowManager()
{
    m_windows.clear();
    if (m_initialized)
    {
        glfwSetErrorCallback(NULL);
        glfwTerminate();
    }
    m_initialized = false;
    DE_LOG(LogGLFWWindowManager, Display, "GLFW library shutdown complete...");
}

void GLFWWindowManager::update()
{
    if (!m_initialized) return;

    glfwPollEvents();

    cleanupClosedWindows();
}

void GLFWWindowManager::cleanupClosedWindows()
{
    auto it = m_windows.begin();
    while (it != m_windows.end())
    {
        if (it->second->ShouldClose())
        {
            DE_LOG(LogGLFWWindowManager, Display, "Window with {} id was deleted", it->first.value);
            it = m_windows.erase(it);
            continue;
        }
        ++it;
    }
}

std::expected<WindowId, WindowCreationError> GLFWWindowManager::createWindow(const WindowSettings& settings)
{
    if (!glfwInit())
    {
        DE_LOG(LogGLFWWindowManager, Error, "Cannot create window. GLFW library is not initialized!");
        return std::unexpected(WindowCreationError::ManagerIsNotInitialized);
    }

    auto window = std::make_shared<GLFWWindow>(settings);
    if (!window->isValid())
    {
        DE_LOG(LogGLFWWindowManager, Error, "Window is not created! pointer is NULL");
        return std::unexpected(WindowCreationError::CreationFailed);
    }

    const WindowId id = m_windowIdCounter++;
    m_windows[id] = window;
    DE_LOG(LogGLFWWindowManager, Display, "added new window with {} id", id.value);

    return id;
}

std::shared_ptr<IWindow> GLFWWindowManager::getWindowById(WindowId& id) const
{
    const auto it = m_windows.find(id);
    return it != m_windows.end() ? it->second : nullptr;
}

bool GLFWWindowManager::areAllWindowsClosed() const
{
    return m_windows.empty();
}
