#pragma once
#include <string_view>
#include <memory>
#include "EngineConfig.h"

namespace Dust
{
class GLFWWindowManager;
class IWindowManager;

class Engine final
{
public:
    Engine(std::unique_ptr<IWindowManager> windowManager);
    ~Engine();

    void Run();

    static constexpr std::string_view version() { return Engine_VERSION_STRING; };

private:
    const std::unique_ptr<IWindowManager> m_windowManager;
    bool m_initialized{false};
};
}  // namespace Dust