#include "Engine.h"
#include "Log/Log.h"
#include <format>
#include "Window/IWindowManager.h"

using namespace Dust;

DEFINE_LOG_CATEGORY_STATIC(LogEngine);

Engine::Engine(std::unique_ptr<IWindowManager> windowManager)  //
    : m_windowManager(std::move(windowManager))
{
    DE_LOG(LogEngine, Display, "..Initializing engine with {} version..", version());

    /*DE_LOG(LogEngine, NoLogging, "No logging example");
    DE_LOG(LogEngine, Display, "Display example");
    DE_LOG(LogEngine, Warning, "Warning example");
    DE_LOG(LogEngine, Error, "Error example");
    DE_LOG(LogEngine, Log, "Logging to file example");
    // DE_LOG(LogEngine, Fatal, "Critical example");*/

    auto windowResult = m_windowManager->createWindow(WindowSettings{});
    if (!windowResult)
    {
        DE_LOG(LogEngine, Error, "Failed to create main window...");
        return;
    }

    if (auto window = m_windowManager->getWindowById(windowResult.value()))
    {
        window->setTitle(std::format("Dust Engine v{}", version()));
    }

    m_initialized = true;
}

Engine::~Engine() = default;

void Engine::Run()
{
    if (!m_initialized)
    {
        DE_LOG(LogEngine, Error, "cannot run... Engine is not initialized...");
        return;
    }

    while (!m_windowManager->areAllWindowsClosed())
    {
        m_windowManager->update();
    }
}
