/**
 * @file        tcp-server-socket.h
 * @brief       Declares the TCP Server socket class for NetFlex.
 *
 * @details
 *  🔹 Inherits from NetFlexSocket to provide TCP-specific server operations.
 *  🔹 Encapsulates logic for binding, listening, accepting, and communicating with clients.
 *
 * Key C++ Concepts Used:
 *  - Inheritance: Inherits common socket behavior from NetFlexSocket.
 *  - Method Overriding: Adds specialized TCP server behavior.
 *  - Templates & STL: Uses `std::pair` to return both client fd and address.
 *  - Encapsulation: Provides public-facing methods with error-safe response types.
 *
 * Real-World Analogy:
 *  - Think of this like a receptionist (server) that listens for visitors (clients), talks to them, and responds.
 *
 * Dependencies:
 *  - Base class: NetFlexSocket
 *  - Utilities: Response<T>, ServerAddressStruct
 */

#pragma once

#include "../../../core/netflex-socket/netflex-socket.h"

#include <utility>

class TcpServerSocket : public NetFlexSocket {
public:
    TcpServerSocket(AddressFamily domain);
    ~TcpServerSocket();

    Response<bool> bindAndListen(const std::string& ip, int port, int backlog = 5);
    Response<pair<SocketFD, ServerAddressStruct>> acceptConnection();
    Response<size_t> sendData(SocketFD clientFd, const string& data);
    Response<string> receiveData(SocketFD clientFd);
};