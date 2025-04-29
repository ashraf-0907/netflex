/**
 * @file        udp-server-socket.h
 * @brief       Declares the UDP Server socket class in NetFlex.
 *
 * @details
 *  🔹 Inherits from NetFlexSocket to support datagram-based (UDP) communication.
 *  🔹 Provides logic to bind to a port, receive data from any client, and respond.
 *  🔹 Unlike TCP, this doesn't manage connections — each message is standalone.
 *
 * Key C++ Concepts Used:
 *  - Inheritance: Shares common socket logic from NetFlexSocket.
 *  - Composition: Uses SocketIOHelper for framed UDP communication.
 *  - STL: Returns structured data via `std::pair`
 *
 * Real-World Analogy:
 *  - Like a postbox that receives letters from anyone, and can send a letter back to any address.
 *
 * Dependencies:
 *  - NetFlexSocket, Response<T>, SocketIOHelper
 */


#pragma once

#include "../../../core/netflex-socket/netflex-socket.h"

class UdpServerSocket : public NetFlexSocket {
public:
    UdpServerSocket(AddressFamily domain);
    ~UdpServerSocket();

    Response<bool> bindToAddress(const string& ip, int port);
    Response<pair<string, ServerAddressStruct>> receiveFrom();
    Response<size_t> sendTo(const string& message, const ServerAddressStruct& destination);
};