/**
 * @file        netflex-socket.cpp
 * @brief       Implementation of the NetFlexSocket base class.
 *
 * @details
 *  🔹 Contains the concrete logic for creating, configuring, and closing sockets.
 *  🔹 Provides default behaviors for all socket types in the NetFlex stack.
 *
 *  Key C++ Concepts Used:
 *  - Constructor Initialization Lists
 *  - Smart resource handling with RAII
 *  - Type-safe enum casting
 *  - Logging and error propagation with custom Response<T>
 *
 *  Real-World Analogy:
 *  - This file is like the operating system interface for all "network ports".
 *
 *  Dependencies:
 *  - Relies on system socket APIs (e.g., socket(), setsockopt(), close())
 *  - Depends on custom response/logger utils from `common/`
 */



#include "netflex-socket.h"


NetFlexSocket::NetFlexSocket(AddressFamily domain, SocketType type, Protocol protocol)
    : socketFd(-1), domain(domain), type(type), protocol(protocol) {}

NetFlexSocket::NetFlexSocket(SocketFD existingFd)
    : socketFd(existingFd), domain(AddressFamily::IPv4), type(SocketType::STREAM), protocol(Protocol::DEFAULT) {} // this is wrong need to be adjusted later with the architecture

NetFlexSocket::~NetFlexSocket() {
    closeSocket();
}

// static_cast<int> used to convert enum class to native C-style int.

Response<bool> NetFlexSocket::createSocket() {
    socketFd = socket(static_cast<int>(domain), static_cast<int>(type), static_cast<int>(protocol));
    if (socketFd < 0) {
        LOG_ERROR("Socket creation failed: " + string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Failed to create socket");
    }
    return Response<bool>::success(true);
}



Response<bool> NetFlexSocket::setSocketOptions() {
    int enable = 1;
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        LOG_ERROR("setsockopt failed on fd " + to_string(socketFd) + ": " + string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "setsockopt failed");
    }
    return Response<bool>::success(true);
}

void NetFlexSocket::closeSocket() {
    if (socketFd != -1) {
        LOG_DEBUG("Closing socket with fd = " + to_string(socketFd));
        close(socketFd);
        socketFd = -1;
    }
}


SocketFD NetFlexSocket::getSocketFd() const {
    return socketFd;
}

ServerAddressStruct NetFlexSocket::getServerAddress() const {
    return serverAddr;
}

ServerAddressStruct NetFlexSocket::getPeerAddress() const {
    return peerAddr;
}


// Why use static_cast<int> on enum class values?
// Answer:
// Because enum class doesn't implicitly convert to int (for type safety).
// But the socket() system call expects int values — so we cast intentionally.