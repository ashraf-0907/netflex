/**
 * @file        udp-client-socket.h
 * @brief       Declares the UDP Client socket class for NetFlex.
 *
 * @details
 *  🔹 Used to send and receive datagrams to/from remote servers.
 *  🔹 Like the server, it’s connectionless — each send includes a destination.
 *  🔹 Receives data from any source and returns sender address info.
 *
 *  Key C++ Concepts Used:
 *  - Inheritance: Shares behavior from NetFlexSocket.
 *  - Composition: Uses SocketIOHelper for message framing.
 *  - Error Handling: Uses Response<T> to return structured results.
 *
 *  Real-World Analogy:
 *  - Like sending a letter to someone with a return address on it — no phone call, no handshake.
 *
 *  Dependencies:
 *  - NetFlexSocket, SocketIOHelper, NetUtils, Response<T>
 */


#pragma once

#include "../../../core/netflex-socket/netflex-socket.h"

class UdpClientSocket : public NetFlexSocket {
public:
    UdpClientSocket(AddressFamily domain);
    ~UdpClientSocket();

    // Response<bool> bindSocket(const std::string& ip, int port);
    Response<size_t> sendTo(const string& message, const string& ip, int port);
    Response<pair<string, ServerAddressStruct>> receiveFrom();
};