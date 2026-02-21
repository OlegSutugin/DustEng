#include "Engine.h"
#include "Log/Log.h"
#include <format>

using namespace Dust;

DEFINE_LOG_CATEGORY_STATIC(LogEngine);

Engine::Engine()
{
    Log::getInstance().log(LogEngine, Dust::LogVerbosity::Display,  //
        std::format("..Initializing engine with {} version..", version()));

    Log::getInstance().log(LogEngine, Dust::LogVerbosity::NoLogging, "No logging example");
    Log::getInstance().log(LogEngine, Dust::LogVerbosity::Display, "Display example");
    Log::getInstance().log(LogEngine, Dust::LogVerbosity::Warning, "Warning example");
    Log::getInstance().log(LogEngine, Dust::LogVerbosity::Error, "Error example");
    Log::getInstance().log(LogEngine, Dust::LogVerbosity::Log, "Logging to file example");
    // Log::getInstance().log(LogEngine, Dust::LogVerbosity::Fatal, "Critical example");
}
