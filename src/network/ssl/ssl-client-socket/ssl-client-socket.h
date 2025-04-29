#pragma once

#include "../../tcp/tcp-client-socket/tcp-client-socket.h"
#include "../ssl-socket.h"
#include "../../../../common/io/socket-io-helper.h"
#include <memory>

class SslClientSocket : public TcpClientSocket {
public:
    SslClientSocket(AddressFamily family);
    ~SslClientSocket();

    Response<bool> connectToServer(const string& host, int port);
    Response<size_t> sendData(const string& message);
    Response<string> receiveData();

    void closeSocket() override;

private:
    unique_ptr<SslSocket> sslSocket;
    unique_ptr<SocketIOHelper> ioHelper;
};
