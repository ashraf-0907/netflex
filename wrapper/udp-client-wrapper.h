#pragma once

#include "../src/network/udp/udp-client-socket/udp-client-socket.h"
#include <memory>

using namespace std;

class UdpClientSocketWrapper {
    public:
        UdpClientSocketWrapper(AddressFamily domain);
    
        Response<size_t> sendMessage(const string& message, const string& ip, int port);
        Response<pair<string, ServerAddressStruct>> receiveMessage();
        void close();
    
    private:
        unique_ptr<UdpClientSocket> udpClient;
    };