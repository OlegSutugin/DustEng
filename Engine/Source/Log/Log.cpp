#include "Log.h"
#include <memory>
#include <unordered_map>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <filesystem>
#include <chrono>

using namespace Dust;

namespace
{
const std::unordered_map<LogVerbosity, spdlog::level::level_enum> c_verbosityMap =  //
    {{LogVerbosity::NoLogging, spdlog::level::level_enum::off},                     //
        {LogVerbosity::Display, spdlog::level::level_enum::info},                   //
        {LogVerbosity::Warning, spdlog::level::level_enum::warn},                   //
        {LogVerbosity::Error, spdlog::level::level_enum::err},                      //
        {LogVerbosity::Log, spdlog::level::level_enum::info},                       //
        {LogVerbosity::Fatal, spdlog::level::level_enum::critical}};

constexpr const char* c_logPattern = "[%H:%M:%S.%e] [%^%l%$] %v";
const std::filesystem::path c_logDirectory = "logs";
constexpr const char* c_logFilePrefix = "Dust";
constexpr const char* c_logFileExtension = "txt";
constexpr const char* c_timestampFormat = "{:%Y.%d.%m-%H.%M.%S}";

}  // namespace

// pImpl
class Log::Impl
{
public:
    Impl()
    {
        const auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        m_consoleLogger = std::make_unique<spdlog::logger>("console", consoleSink);
        m_consoleLogger->set_pattern(c_logPattern);

        const auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(makeLogFile().string(), true);
        m_fileLogger = std::make_unique<spdlog::logger>("file", fileSink);
        m_fileLogger->set_pattern(c_logPattern);
    }

    void log(LogVerbosity verbosity, const std::string& message) const
    {
        if (verbosity == LogVerbosity::NoLogging) return;

        const auto spdLevelIt = c_verbosityMap.find(verbosity);
        if (spdLevelIt == c_verbosityMap.end()) return;

        const auto spdLevel = spdLevelIt->second;
        if (verbosity != Dust::LogVerbosity::Log)
        {
            m_consoleLogger->log(spdLevel, message);
        }
        m_fileLogger->log(spdLevel, message);

        if (verbosity == Dust::LogVerbosity::Fatal)
        {
            PLATFORM_BREAK();
        }
    }

private:
    std::unique_ptr<spdlog::logger> m_consoleLogger;
    std::unique_ptr<spdlog::logger> m_fileLogger;

    std::filesystem::path makeLogFile() const
    {
        std::filesystem::create_directory(c_logDirectory);
        const auto now = std::chrono::system_clock::now();
        const auto nowSeconds = std::chrono::floor<std::chrono::seconds>(now);
        const std::string timestamp = std::format(c_timestampFormat, nowSeconds);
        const std::string logName = std::format("{}-{}.{}", c_logFilePrefix, timestamp, c_logFileExtension);
        return c_logDirectory / logName;
    }
};
// interface

Log::Log() : m_pImpl(std::make_unique<Impl>())
{
    //
}
Log::~Log() = default;

void Log::log(const LogCategory& category, LogVerbosity verbosity, const std::string& message) const
{
    m_pImpl->log(verbosity, std::format("[{}] {}", category.name(), message));
}
