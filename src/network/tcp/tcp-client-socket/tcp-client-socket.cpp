/**
 * @file        tcp-client-socket.cpp
 * @brief       Implements the TCP Client logic for NetFlex.
 *
 * @details
 *  🔹 Connects to a remote server using a specified IP and port.
 *  🔹 Uses NetFlex’s internal framing and Response<T> for safe I/O.
 *  🔹 Supports both client-initiated and server-accepted wrapping logic.
 *
 * Key C++ Concepts Used:
 *  - Constructor Overloading
 *  - Member Initialization Lists
 *  - System socket functions: connect()
 *  - Composition (uses SocketIOHelper for I/O)
 *
 * Real-World Analogy:
 *  - A walkie-talkie calling another unit (connect), sending and receiving messages.
 *
 * Dependencies:
 *  - SocketIOHelper, NetUtils, Logger, Response<T>
 */


#include "tcp-client-socket.h"
#include "../../../../common/logger/logger.h"

TcpClientSocket::TcpClientSocket(AddressFamily domain)
    : NetFlexSocket(domain, SocketType::STREAM, Protocol::DEFAULT) {

        createSocket();
        setSocketOptions();
    }

TcpClientSocket::TcpClientSocket(SocketFD clientFd, const ServerAddressStruct& clientAddress)
    : NetFlexSocket(clientFd), clientAddr(clientAddress) {
        createSocket();
        setSocketOptions();
    }

TcpClientSocket::~TcpClientSocket() {
    closeSocket();
}

Response<bool> TcpClientSocket::connectToServer(const std::string& ipAddress, int port) {

    ServerAddressStruct serverAddress;
    if (!NetUtils::toSockAddr(ipAddress, port, serverAddress)) {
        LOG_ERROR("Invalid IP address or port");
        return Response<bool>::error(SocketStatus::INVALID_ARGUMENT, "Invalid IP or port");
    }

    int result = connect(socketFd, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result < 0) {
        LOG_ERROR("Connection to " + ipAddress + ":" + std::to_string(port) + " failed: " + std::string(strerror(errno)));
        return Response<bool>::error(SocketStatus::CONNECTION_FAILED, "Connection failed");
    }

    this->serverAddr = serverAddress;
    return Response<bool>::success(true);
}

Response<size_t> TcpClientSocket::sendData(const string& data) {
    return SocketIOHelper::sendFramed(this->getSocketFd(), data);
}

Response<std::string> TcpClientSocket::receiveData() {
    Response<string> response = SocketIOHelper::recvFramed(this->socketFd);
    return response;
}
