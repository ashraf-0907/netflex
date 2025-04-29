#pragma once

#include "../src/network/tcp/tcp-server-socket/tcp-server-socket.h"
#include <memory>

class TcpServerSocketWrapper {
public:
    TcpServerSocketWrapper(AddressFamily domain);
    
    Response<bool> bindAndListen(const string& host, int port, int backlog = 10);
    Response<pair<int, sockaddr_storage>> acceptConnection();
    Response<string> receive(int clientFd);
    Response<size_t> send(int clientFd, const string& message);
    void closeSocket();
    int getSocketFd() const;

private:
    unique_ptr<TcpServerSocket> serverSocket;
};
