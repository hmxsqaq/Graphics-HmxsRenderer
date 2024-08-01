#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <mutex>
#include <sstream>

#define LOG_ERROR(message) Log::Instance().LogMessage(Log::Level::LOG_ERROR, message)
#define LOG_WARNING(message) Log::Instance().LogMessage(Log::Level::LOG_WARNING, message)
#define LOG_INFO(message) Log::Instance().LogMessage(Log::Level::LOG_INFO, message)
#define LOG_DEBUG(message) Log::Instance().LogMessage(Log::Level::LOG_DEBUG, message)

class Log {
public:
    enum class Level {
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO,
        LOG_DEBUG
    };

    static Log& Instance() {
        static Log instance;
        return instance;
    }

    void SetLogLevel(const Level level) {
        std::lock_guard lock(mutex_);
        log_level_ = level;
    }

    void SetLogFile(const std::string& filename, const std::ios_base::openmode mode = std::ios::out | std::ios::app) {
        std::lock_guard lock(mutex_);
        if (log_file_.is_open()) {
            log_file_.close();
        }
        log_file_.open(filename, mode);
        if (!log_file_) {
            throw std::runtime_error("Unable to open log file: " + filename);
        }
    }

    void LogMessage(const Level level, const std::string& message) {
        if (level > log_level_) return;

        std::string log_message;
        {
            const std::string timestamp = GetCurrentTimestamp();
            const std::string level_str = LevelToString(level);
            std::ostringstream oss;
            oss << "[" << timestamp << "] [" << level_str << "] " << message << "\n";
            log_message = oss.str();
        }
        {
            std::lock_guard lock(mutex_);
            std::cout << log_message;
            if (log_file_.is_open()) {
                log_file_ << log_message;
            }
        }
    }

    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

private:
    Log() : log_level_(Level::LOG_INFO) {};

    static std::string GetCurrentTimestamp() {
        const auto now = std::chrono::system_clock::now();
        const auto now_c = std::chrono::system_clock::to_time_t(now);
        const auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::tm now_tm{};
        localtime_s(&now_tm, &now_c);
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
        return oss.str();
    }

    static std::string LevelToString(const Level level) {
        switch (level) {
            case Level::LOG_ERROR:      return "ERROR";
            case Level::LOG_WARNING:    return "WARNING";
            case Level::LOG_INFO:       return "INFO";
            case Level::LOG_DEBUG:      return "DEBUG";
            default: return "UNKNOWN";
        }
    }

    Level log_level_;
    std::ofstream log_file_;
    std::mutex mutex_;
};

#endif //LOG_H
