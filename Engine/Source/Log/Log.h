#pragma once
#include <string>
#include <string_view>
#include <memory>
#include <concepts>
#include <source_location>
#include <format>
#include "Core/Utility.h"

namespace Dust
{
enum class LogVerbosity : uint8_t
{
    NoLogging = 0,
    Display,
    Warning,
    Error,
    Log,
    Fatal
};

struct LogCategory
{
    explicit LogCategory(const std::string& name) : m_name(name) {}
    std::string name() const { return m_name; }

private:
    const std::string m_name{};
};

class Log final : public NonCopyable
{
public:
    static Log& getInstance()
    {
        static Log instance;
        return instance;
    }

    void log(const LogCategory& category,  //
        LogVerbosity verbosity,            //
        const std::string& message,        //
        bool showLocation = false,         //
        const std::source_location location = std::source_location::current()) const;

private:
    Log();
    ~Log();

    class Impl;
    std::unique_ptr<Impl> m_pImpl;
};

constexpr LogVerbosity c_minVerbosity = LogVerbosity::Display;
constexpr LogVerbosity c_maxVerbosity = LogVerbosity::Fatal;

// concepts
template <typename T>
concept ValidLogCategory = std::constructible_from<LogCategory, T>;

template <typename T>
concept LoggableMessage = std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view>;

template <LogVerbosity V>
concept ValidVerbosityLevel = V == LogVerbosity::NoLogging ||  //
                              V == LogVerbosity::Display ||    //
                              V == LogVerbosity::Warning ||    //
                              V == LogVerbosity::Error ||      //
                              V == LogVerbosity::Log ||        //
                              V == LogVerbosity::Fatal;

}  // namespace Dust

#define DEFINE_LOG_CATEGORY_STATIC(logName)    \
    namespace                                  \
    {                                          \
    const Dust::LogCategory logName(#logName); \
    }

#define LE_LOG_IMPL(categoryName, verbosity, showLocation, formatStr, ...)                                                            \
    do                                                                                                                                \
    {                                                                                                                                 \
        if constexpr (Dust::LogVerbosity::verbosity >= Dust::c_minVerbosity && Dust::LogVerbosity::verbosity <= Dust::c_maxVerbosity) \
        {                                                                                                                             \
            static_assert(Dust::ValidVerbosityLevel<Dust::LogVerbosity::verbosity>,                                                   \
                "Verbosity should be one of NoLogging, Display, Warning, Error, Log, Fatal");                                         \
            static_assert(Dust::ValidLogCategory<decltype(categoryName)>, "Category must be of a type LogCategory");                  \
            static_assert(Dust::LoggableMessage<decltype(formatStr)>, "Log message should be convertable to string");                 \
            Dust::Log::getInstance().log(                                                                                             \
                categoryName, Dust::LogVerbosity::verbosity, std::format(formatStr __VA_OPT__(, ) __VA_ARGS__), showLocation);        \
        }                                                                                                                             \
    } while (0)

#define DE_LOG(categoryName, verbosity, formatStr, ...) LE_LOG_IMPL(categoryName, verbosity, false, formatStr, __VA_ARGS__);

#define DE_LOG_DEBUG(categoryName, verbosity, formatStr, ...) LE_LOG_IMPL(categoryName, verbosity, true, formatStr, __VA_ARGS__);