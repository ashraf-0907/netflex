#pragma once

#include "../src/network/ssl/ssl-client-socket/ssl-client-socket.h"
#include <memory>
#include <string>

class SslClientSocketWrapper {
public:
    SslClientSocketWrapper(AddressFamily domain, const string& serverIp, int port);

    Response<size_t> sendMessage(const string& message);
    Response<string> receiveMessage();
    void close();

private:
    unique_ptr<SslClientSocket> sslClient;
};
