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
        const auto spdLevel = c_verbosityMap.at(verbosity);

        if (verbosity != Dust::LogVerbosity::Log && m_consoleLogger->should_log(spdLevel))
        {
            m_consoleLogger->log(spdLevel, message);
        }

        if (m_fileLogger->should_log(spdLevel))
        {
            m_fileLogger->log(spdLevel, message);
        }

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
        const auto now = std::chrono::system_clock::now();
        const auto nowSeconds = std::chrono::floor<std::chrono::seconds>(now);
        const std::string timestamp = std::format(c_timestampFormat, nowSeconds);
        const std::string logName = std::format("{}-{}.{}", c_logFilePrefix, timestamp, c_logFileExtension);

        std::error_code errorCode;
        std::filesystem::create_directory(c_logDirectory, errorCode);
        if (errorCode)
        {
            const auto logDir = std::filesystem::current_path() / c_logDirectory;
            m_consoleLogger->log(spdlog::level::err, std::format("Failed to create log directory: {}", logDir.string()));

            const auto defaultLogDir = std::filesystem::current_path() / logName;
            m_consoleLogger->log(
                spdlog::level::warn, std::format("Will write to file in the current directory: {}", defaultLogDir.string()));
            return std::filesystem::path(logName);
        }

        return c_logDirectory / logName;
    }
};
// interface

Log::Log() : m_pImpl(std::make_unique<Impl>())
{
    //
}
Log::~Log() = default;

void Log::log(const LogCategory& category,  //
    LogVerbosity verbosity,                 //
    const std::string& message,             //
    bool showLocation,                      //
    const std::source_location location) const
{
    const std::string fmtMsg = showLocation                                                                                               //
                                   ? std::format("[{}] [{}:{}] {}", category.name(), location.function_name(), location.line(), message)  //
                                   : std::format("[{}] {}", category.name(), message);

    m_pImpl->log(verbosity, fmtMsg);
}
