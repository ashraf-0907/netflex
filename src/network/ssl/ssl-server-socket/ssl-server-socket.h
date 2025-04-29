#pragma once

#include "../../tcp/tcp-server-socket/tcp-server-socket.h"
#include "../ssl-socket.h"
#include <unordered_map>
#include <memory>

class SslServerSocket : public TcpServerSocket {
public:
    SslServerSocket(AddressFamily domain,const string &certPath,const string &keyPath);
    ~SslServerSocket();

    Response<bool> bindAndListen(const string& ip, int port, int backlog = 10);
    Response<SocketFD> acceptSslConnection();
    Response<size_t> sendData(SocketFD clientFd, const string& message);
    Response<string> receiveData(SocketFD clientFd);

    void closeSocket() override;

private:
    unique_ptr<SocketIOHelper> ioHelper;
    unordered_map<SocketFD, unique_ptr<SslSocket>> clientSslMap;

    string certPath;
    string keyPath;
};
