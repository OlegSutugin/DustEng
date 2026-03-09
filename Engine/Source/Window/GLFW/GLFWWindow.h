#pragma once
#include <string>
#include <functional>
#include "Window/IWindow.h"
#include "Core/Utility.h"

class GLFWwindow;

namespace Dust
{
class GLFWWindow : public IWindow, public NonCopyable
{
public:
    GLFWWindow(const WindowSettings& settings);
    ~GLFWWindow() override;

    void setTitle(const std::string& title) override;
    bool isValid() const override;
    bool ShouldClose() const override;

private:
    GLFWwindow* m_window{nullptr};
};
}  // namespace Dust
