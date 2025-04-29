/**
 * @file        udp-server-socket.cpp
 * @brief       Implements the UDP Server logic for NetFlex.
 *
 * @details
 *  🔹 Binds to a UDP port and waits for messages from any sender.
 *  🔹 Uses helper functions for framed communication and response.
 *  🔹 Stateless, connectionless model — fits UDP's design perfectly.
 *
 *  Key C++ Concepts Used:
 *  - Inheritance with constructor initializer list
 *  - Composition with SocketIOHelper
 *  - Error-safe programming using Response<T>
 *  - System networking functions: bind(), recvfrom(), sendto()
 *
 *  Real-World Analogy:
 *  - Like a radio receiver: anyone can send a message, and you can reply to any address.
 *
 *  Dependencies:
 *  - Logger, NetUtils, SocketIOHelper, Response<T>
 */


#include "udp-server-socket.h"
#include "../../../../common/logger/logger.h"

UdpServerSocket::UdpServerSocket(AddressFamily domain)
    : NetFlexSocket(domain, SocketType::DATAGRAM, Protocol::DEFAULT) {
        createSocket();
    }

UdpServerSocket::~UdpServerSocket() {
    closeSocket();
}

Response<bool> UdpServerSocket::bindToAddress(const string& ip, int port) {

    if (!NetUtils::toSockAddr(ip, port, serverAddr)) {
        LOG_ERROR("Invalid bind address");
        return Response<bool>::error(SocketStatus::INVALID_ARGUMENT, "Invalid IP/Port");
    }

    if (bind(socketFd, reinterpret_cast<const sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
        LOG_ERROR("UDP bind failed: " + string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Bind failed");
    }

    LOG_INFO("UDP server bound to " + ip + ":" + to_string(port));
    return Response<bool>::success(true);
}

Response<pair<string, ServerAddressStruct>> UdpServerSocket::receiveFrom() {
    return SocketIOHelper::recvFramedFrom(this->getSocketFd());
}

Response<size_t> UdpServerSocket::sendTo(const string& message, const ServerAddressStruct& destination) {
    return SocketIOHelper::sendFramedTo(this->getSocketFd(), message, &destination);
}
