#pragma once

#include <iostream>
#include <string>
#include <mutex>

using namespace std;

/**
 * @file Logger.h
 * @brief Implements a Singleton-based logging system for debugging, error handling, and tracing.
 *
 * 🔹 Why We Need This:
 * - Centralized logging prevents scattered `cout` and `cerr` statements.
 * - Helps in debugging, monitoring, and performance analysis.
 * - Supports different logging levels (INFO, WARNING, ERROR).
 *
 * 🔹 Key Concepts:
 * - **Singleton Pattern**: Ensures only one logger instance exists.
 * - **Encapsulation**: The class controls its own behavior without exposing implementation details.
 * - **Extensibility**: Can be modified later to support file logging, remote logging, etc.
 */

enum class LogLevel
{
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

class Logger{

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger();

    static Logger &getInstance();

    void setLogLevel(LogLevel level);
    void log(LogLevel level, const string &message, const char *file, int line);

private:
    LogLevel currentLevel;
    mutex logMutex;
};

#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::ERROR, msg, __FILE__, __LINE__)
#define LOG_WARNING(msg) Logger::getInstance().log(LogLevel::WARNING, msg, __FILE__, __LINE__)
#define LOG_INFO(msg) Logger::getInstance().log(LogLevel::INFO, msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG, msg, __FILE__, __LINE__)