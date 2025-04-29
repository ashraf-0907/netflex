#include "./logger.h"

/**
 * @file Logger.cpp
 * @brief Implements the Singleton Logger.
 *
 * 🔹 How It Works:
 * - `getInstance()` returns the single logger instance.
 * - `info()` logs general messages to `stdout`.
 * - `warning()` logs warnings to `stderr` but does not stop execution.
 * - `error()` logs critical errors to `stderr` and optionally terminates the program.
 */


Logger::Logger(): currentLevel(LogLevel::INFO) {}

Logger& Logger::getInstance(){

    static Logger instance; // lazy initilzation (created when first called)
    return instance;
}

void Logger::setLogLevel(LogLevel level) {

    lock_guard<mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::log(LogLevel level, const string& message, const char* file, int line) {

    lock_guard<mutex> lock(logMutex);
    if (level > currentLevel) return;

    string levelStr;
    switch (level) {
        case LogLevel::ERROR: levelStr = "[ERROR]"; break;
        case LogLevel::WARNING: levelStr = "[WARN]"; break;
        case LogLevel::INFO: levelStr = "[INFO]"; break;
        case LogLevel::DEBUG: levelStr = "[DEBUG]"; break;
    }

    cerr << levelStr << " " << file << ":" << line << " - " << message << endl;
}