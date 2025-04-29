/**
 * @file        tcp-server-socket.cpp
 * @brief       Implements the TCP Server socket logic for NetFlex.
 *
 * @details
 *  🔹 Handles bind(), listen(), accept() operations on a TCP server socket.
 *  🔹 Uses framing helpers to send/receive data over accepted connections.
 *  🔹 Extends NetFlexSocket and applies system-level socket calls safely.
 *
 * Key C++ Concepts Used:
 *  - Inheritance + constructor initializer list
 *  - Error-safe programming with Response<T>
 *  - System socket APIs: bind, listen, accept
 *  - Composition: uses SocketIOHelper (composition over inheritance)
 *
 * Real-World Analogy:
 *  - Acts like a doorman that opens the connection, receives a guest, and begins conversation.
 *
 * Dependencies:
 *  - Logger, Response<T>, SocketIOHelper, NetUtils
 */


#include "./tcp-server-socket.h"
#include "../../../../common/logger/logger.h"

TcpServerSocket::TcpServerSocket(AddressFamily domain)
    : NetFlexSocket(domain, SocketType::STREAM, Protocol::DEFAULT) {
        auto socketRes = createSocket();
        if (!socketRes.ok()){
            LOG_ERROR("Failed to create socket");
        }
        
        auto optRes = setSocketOptions();
        if (!optRes.ok()){
            LOG_ERROR("Failed to set options");
        }
    }

TcpServerSocket::~TcpServerSocket() {
    closeSocket();
}

Response<bool> TcpServerSocket::bindAndListen(const std::string& ip, int port, int backlog) {

    if (!NetUtils::toSockAddr(ip, port, serverAddr)) {
        LOG_ERROR("Failed to create bind address");
        return Response<bool>::error(SocketStatus::INVALID_ARGUMENT, "Bind address invalid");
    }

    // reinterpret_cast is used to cast custom struct to system-level sockaddr*.

    if (bind(socketFd, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        LOG_ERROR("Bind failed: " + string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Bind failed");
    }

    if (listen(socketFd, backlog) < 0) {
        LOG_ERROR("Listen failed: " + string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Listen failed");
    }

    LOG_INFO("Listening on " + ip + ":" + to_string(port));
    return Response<bool>::success(true);
}


Response<pair<SocketFD, ServerAddressStruct>> TcpServerSocket::acceptConnection() {
    ServerAddressStruct clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    //  Calls the system accept() and populates clientAddr.
    SocketFD clientFd = accept(socketFd, reinterpret_cast<sockaddr*>(&clientAddr), &addrLen);
    if (clientFd < 0) {
        LOG_ERROR("Accept failed: " + string(strerror(errno)));
        return Response<pair<SocketFD, ServerAddressStruct>>::error(SocketStatus::UNKNOWN_ERROR, "Accept failed");
    }

    LOG_INFO("Accepted connection with fd = " + to_string(clientFd));
    return Response<pair<SocketFD, ServerAddressStruct>>::success({clientFd, clientAddr});
}

Response<size_t> TcpServerSocket::sendData(SocketFD clientFd, const string& data) {
    return SocketIOHelper::sendFramed(clientFd, data);
}

Response<string> TcpServerSocket::receiveData(SocketFD clientFd) {
    return SocketIOHelper::recvFramed(clientFd);
}

// Why is reinterpret_cast used with sockaddr?
// Answer: C++ has stricter type rules.
// We must cast our sockaddr_in (IPv4) or custom ServerAddressStruct to sockaddr* because that’s what system calls 
//(bind, accept) expect.

/*
What’s the benefit of SocketIOHelper::sendFramed()?
Answer: It sends messages with a length prefix. Without it, TCP would behave like a stream, 
and you wouldn’t know where a message ends. This makes your protocol reliable and structured.
*/