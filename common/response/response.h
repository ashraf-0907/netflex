#pragma once

#include <string>

using namespace std;

enum class SocketStatus {
    SUCCESS,
    TIMEOUT,
    CONNECTION_FAILED,
    HANDSHAKE_FAILED,
    INVALID_ARGUMENT,
    WRITE_ERROR,
    READ_ERROR,
    UNKNOWN_ERROR,
    CERTIFICATE_ERROR,
};
 
inline string getStatusString(SocketStatus status) {
    switch (status) {
        case SocketStatus::SUCCESS: return "Success";
        case SocketStatus::TIMEOUT: return "Timeout";
        case SocketStatus::CONNECTION_FAILED: return "Connection Failed";
        case SocketStatus::HANDSHAKE_FAILED: return "Handshake Failed";
        case SocketStatus::INVALID_ARGUMENT: return "Invalid Argument";
        case SocketStatus::WRITE_ERROR: return "Write Error";
        case SocketStatus::READ_ERROR: return "Read Error";
        case SocketStatus::CERTIFICATE_ERROR: return "Certificate Error";
        case SocketStatus::UNKNOWN_ERROR: default: return "Unknown Error";
    }
}

template <typename T>
struct Response {
    SocketStatus status;
    T data;
    string message;

    bool ok() const { return status == SocketStatus::SUCCESS; }

    static Response<T> success(const T& data) {
        return {SocketStatus::SUCCESS, data, "Success"};
    }

    static Response<T> error(SocketStatus code, const string& msg = "") {
        return {code, T{}, msg.empty() ? getStatusString(code) : msg};
    }
};
