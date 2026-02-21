#include "Log.h"
#include <memory>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace Dust;

namespace
{
const std::unordered_map<LogVerbosity, spdlog::level::level_enum> c_verbosityMap =  //
    {{LogVerbosity::NoLogging, spdlog::level::level_enum::off},                     //
        {LogVerbosity::Display, spdlog::level::level_enum::info},                   //
        {LogVerbosity::Warning, spdlog::level::level_enum::warn},                   //
        {LogVerbosity::Error, spdlog::level::level_enum::err},                      //
        {LogVerbosity::Fatal, spdlog::level::level_enum::critical}};
}

// pImpl
class Log::Impl
{
public:
    Impl()
    {
        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_consoleLogger = std::make_unique<spdlog::logger>("console", consoleSink);
    }

    void log(LogVerbosity verbosity, const std::string& message) const
    {
        if (verbosity == LogVerbosity::NoLogging) return;

        const auto spdLevel = c_verbosityMap.find(verbosity);
        if (spdLevel != c_verbosityMap.end())
        {
            m_consoleLogger->log(spdLevel->second, message);
        }
    }

private:
    std::unique_ptr<spdlog::logger> m_consoleLogger;
};
// interface

Log::Log() : m_pImpl(std::make_unique<Impl>())
{
    //
}
Log::~Log() = default;

void Log::log(LogVerbosity verbosity, const std::string& message) const
{
    m_pImpl->log(verbosity, message);
}
