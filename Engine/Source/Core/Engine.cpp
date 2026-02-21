#include "Engine.h"
#include "Log/Log.h"
#include <format>

using namespace Dust;

Engine::Engine()
{
    Log::getInstance().log(Dust::LogVerbosity::Display,  //
        std::format("..Initializing engine with {} version..", version()));

    Log::getInstance().log(Dust::LogVerbosity::NoLogging, "No logging example");
    Log::getInstance().log(Dust::LogVerbosity::Display, "Display example");
    Log::getInstance().log(Dust::LogVerbosity::Warning, "Warning example");
    Log::getInstance().log(Dust::LogVerbosity::Error, "Error example");
    Log::getInstance().log(Dust::LogVerbosity::Fatal, "Critical example");
}
