/**
 * @file        netflex-socket.h
 * @brief       Base abstract class for all socket types in NetFlex.
 *
 * @details
 *  🔹 Defines the foundational structure for all socket wrappers (TCP, UDP, SSL).
 *  🔹 Handles basic properties such as domain, socket type, protocol, and file descriptor.
 *  🔹 Exposes a uniform interface and enforces polymorphism via virtual functions.
 *
 *  Key C++ Concepts Used:
 *  - Inheritance: This is the base class from which all socket classes inherit.
 *  - RAII: Resource cleanup is handled in destructors.
 *  - Virtual Functions: Enables derived classes to override socket-specific logic.
 *  - Access Specifiers: clear use of protected/public to enforce encapsulation.
 *
 *  Real-World Analogy:
 *  - Think of this as a base "Device Driver" class for all types of network hardware.
 *
 *  Dependencies:
 *  - Uses enums like AddressFamily, SocketType, Protocol (from common enums).
 *  - Used by: TcpServerSocket, UdpClientSocket, SslSocket, etc.
 */

#pragma once

#include <string>
#include "../../../common/response/response.h"
#include "../../../common/logger/logger.h"
#include "../../../common/utils/net-utils/net-utils.h"
#include "../../../common/utils/socket-defines.h"
#include "../../../common/io/socket-io-helper.h"
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

using namespace std;


class NetFlexSocket {
protected:
    SocketFD socketFd;
    AddressFamily domain;
    SocketType type;
    Protocol protocol;
    ServerAddressStruct serverAddr;
    ServerAddressStruct peerAddr;

public:
    NetFlexSocket(AddressFamily domain, SocketType type, Protocol protocol);
    NetFlexSocket(SocketFD existingFd);   // Used when wrapping an accepted socket
    virtual ~NetFlexSocket();    // Virtual for polymorphic deletion

    Response<bool> createSocket();
    Response<bool> setSocketOptions(); // Set things like reuseaddr, non-blocking
    virtual void closeSocket();

    SocketFD getSocketFd() const;
    ServerAddressStruct getServerAddress() const;
    ServerAddressStruct getPeerAddress() const;
};