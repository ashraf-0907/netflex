// #include "../common/index.h"

// /**
//  * @file test-logger.cpp
//  * @brief Tests Logger and LoggerDecorator functionalities.
//  *
//  * 🔹 What We Are Testing:
//  * - `info()`, `warning()`, and `error()` should be colorized.
//  * - `LoggerDecorator` should wrap function calls and automatically detect issues.
//  */

// // Sample function (No return value)
// void successfulFunction() {
//     Logger::getInstance().info("This function runs without issues.");
// }

// // Sample function that returns a warning
// int warningFunction() {
//     return 1;  // Non-zero positive value (Warning)
// }

// // Sample function that returns an error
// int errorFunction() {
//     return -1;  // Negative value (Error)
// }

// // Sample function that throws an exception
// void exceptionFunction() {
//     throw runtime_error("Something went wrong!");
// }

// int main() {
//     // INFO (Normal execution)
//     LoggerDecorator::logExecution("Successful Function Execution", successfulFunction);

//     // WARNING (Returns warning code)
//     LoggerDecorator::logExecution("Warning Function Execution", warningFunction);

//     // ERROR (Returns error code)
//     LoggerDecorator::logExecution("Error Function Execution", errorFunction);

//     // EXCEPTION (Throws an error)
//     LoggerDecorator::logExecution("Exception Function Execution", exceptionFunction);

//     return 0;
// }
