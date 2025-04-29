/**
 * @file        tcp-client-socket.h
 * @brief       Declares the TCP Client class for connecting to servers.
 *
 * @details
 *  🔹 Inherits from NetFlexSocket and handles client-side TCP operations.
 *  🔹 Enables connecting to remote servers, sending, and receiving framed data.
 *
 *  Key C++ Concepts Used:
 *  - Inheritance: Derives from base socket class.
 *  - Constructor Overloading: One for new clients, one for accepted client sockets.
 *  - Member Initialization: Stores remote server/client address state.
 *  - Composition: Uses SocketIOHelper internally.
 *
 * Real-World Analogy:
 *  - Like a phone that dials a remote number (connect) and then exchanges messages.
 *
 * Dependencies:
 *  - NetFlexSocket, SocketIOHelper, Response<T>
 */


#pragma once

#include "../../../core/netflex-socket/netflex-socket.h"
#include <string>

class TcpClientSocket : public NetFlexSocket {
public:
    TcpClientSocket(AddressFamily domain);

    /*Used when you accept a connection on the server side and want to wrap the client socket in a 
    TcpClientSocket (e.g., in SSL).*/
    TcpClientSocket(SocketFD clientFd, const ServerAddressStruct &clientAddress);
    ~TcpClientSocket();

    Response<bool> connectToServer(const string& ipAddress, int port);
    Response<size_t> sendData(const string& data);
    Response<string> receiveData();

private:
    ServerAddressStruct clientAddr;

    // Response<size_t> sendAll(const string& data);
};
