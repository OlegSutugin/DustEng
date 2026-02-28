#pragma once
#include "GLFWWindow.h"
#include <memory>
#include <unordered_map>
#include <expected>

namespace Dust
{
class GLFWWindow;
enum class WindowCreationError : uint8_t
{
    ManagerIsNotInitialized,
    CreationFailed
};

class GLFWWindowManager final
{
public:
    GLFWWindowManager();
    ~GLFWWindowManager();

    void update();
    std::expected<WindowId, WindowCreationError> createWindow(const WindowSettings& settings);
    std::shared_ptr<GLFWWindow> getWindowById(WindowId& id) const;
    bool areAllWindowsClosed() const;

private:
    bool m_initialized{false};
    std::unordered_map<WindowId, std::shared_ptr<GLFWWindow>> m_windows;
    void cleanupClosedWindows();
    WindowId m_windowIdCounter{1};
};

}  // namespace Dust