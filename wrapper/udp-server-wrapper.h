#pragma once

#include "../src/network/udp/udp-server-socket/udp-server-socket.h"

using namespace std;

class UdpServerSocketWrapper {
public:
    UdpServerSocketWrapper(AddressFamily domain);
    ~UdpServerSocketWrapper();

    Response<bool> bindToAddress(const string& ip, int port);
    Response<pair<string, ServerAddressStruct>> receiveFrom();
    Response<size_t> sendTo(const string& message, const ServerAddressStruct &clientAddr);

private:
    unique_ptr<UdpServerSocket> server;
};
