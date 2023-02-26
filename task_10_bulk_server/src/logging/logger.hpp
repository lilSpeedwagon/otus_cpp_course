#pragma once

#include <chrono>
#include <string>

#include <boost/noncopyable.hpp>


namespace bulk_server::logging {

// forward decalration
class Logger;

/// @enum Log level
enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
};

/// @brief A single log entry.
struct LogMsg {
    std::string message{};
    LogLevel level{};
    std::chrono::system_clock::time_point timepoint{};
};

/// @class Utility class for stream-like logging.
/// Create the entry of the class as a temporary object
/// and all consumed log data will be written to logger in the 
/// end of the entry lifetime.
/// Consider to use this class via LOG() defines insted of a manual usage.
class LogHolder : public boost::noncopyable {
public:
    LogHolder(LogLevel level);
    ~LogHolder();

    template<typename T, typename std::enable_if<std::is_arithmetic_v<T>, bool>::type = true>
    LogHolder& operator<<(const T& val) {
        log_entry_.message += std::to_string(val);
        return *this;
    }

    LogHolder& operator<<(const std::string& msg) {
        log_entry_.message += msg;
        return *this;
    }

private:
    LogHolder(LogHolder&&);
    LogHolder& operator=(LogHolder&&);

    LogMsg log_entry_;
};

/// @brief macros for stream like logging.
/// Example: LOG_INFO() << "log message";
#define LOG(lvl) bulk_server::logging::LogHolder((lvl))
#define LOG_DEBUG() LOG(bulk_server::logging::LogLevel::Debug)
#define LOG_INFO() LOG(bulk_server::logging::LogLevel::Info)
#define LOG_WARNING() LOG(bulk_server::logging::LogLevel::Warning)
#define LOG_ERROR() LOG(bulk_server::logging::LogLevel::Error)

/// @brief Default console logger singletone.
class Logger : public boost::noncopyable {
public:
    static Logger& GetInstance();

    void Log(const LogMsg& msg);
    
    void LogLevel(LogLevel level);
    logging::LogLevel LogLevel() const;

private:
    Logger() = default;
    Logger(Logger&&);
    Logger& operator=(Logger&&);

    logging::LogLevel level_ = LogLevel::Debug;
};

} // namespace bulk_server::logging
