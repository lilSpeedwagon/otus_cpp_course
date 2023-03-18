#include "logger.hpp"

#include <iostream>
#include <sstream>

#include <utils/time.hpp>


namespace join_server::logging {

namespace {

std::string Format(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:
            return "DEBUG:";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARN";
        case LogLevel::Error:
            return "ERROR";
        default:
            throw std::logic_error("unknown log level");
    }
}

std::string Format(const LogMsg& msg) {
    std::stringstream ss;
    ss << utils::GetTimeString(msg.timepoint) << "\t\t";
    ss << Format(msg.level) << "\t\t";
    ss << msg.message;
    return ss.str();
}

} // namespace

LogHolder::LogHolder(LogLevel level) {
    log_entry_.level = level;
    log_entry_.timepoint = std::chrono::system_clock::now();
}

LogHolder::~LogHolder() {
    if (!log_entry_.message.empty()) {
        try {
            auto& logger = Logger::GetInstance();
            logger.Log(log_entry_);
        } catch (const std::exception& ex) {
            std::cerr << "unable to log entry: " << ex.what();
        }
    }
}

Logger& Logger::GetInstance() {
    static Logger logger;
    return logger;
}

void Logger::Log(const LogMsg& msg) {
    if (msg.level >= level_) {
        std::cout << Format(msg) << '\n';
    }
}

void Logger::LogLevel(logging::LogLevel level) {
    level_ = level;
}

logging::LogLevel Logger:: LogLevel() const {
    return level_;
}

} // namespace join_server::logging