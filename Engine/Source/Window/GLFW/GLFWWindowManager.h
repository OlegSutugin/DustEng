#pragma once
#include "Window/IWindow.h"
#include <memory>
#include <unordered_map>
#include <expected>
#include "Window/IWindowManager.h"
#include "Core/Utility.h"

namespace Dust
{
class GLFWWindowManager final : public IWindowManager, public NonCopyable
{
public:
    GLFWWindowManager();
    ~GLFWWindowManager() override;

    void update() override;
    std::expected<WindowId, WindowCreationError> createWindow(const WindowSettings& settings) override;
    std::shared_ptr<IWindow> getWindowById(WindowId& id) const override;
    bool areAllWindowsClosed() const override;

private:
    bool m_initialized{false};
    std::unordered_map<WindowId, std::shared_ptr<IWindow>> m_windows;
    void cleanupClosedWindows();
    WindowId m_windowIdCounter{1};
};

}  // namespace Dust