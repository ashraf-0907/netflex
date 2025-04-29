#pragma once

#include "../src/network/ssl/ssl-server-socket/ssl-server-socket.h"
#include <memory>
#include <string>

class SslServerSocketWrapper {
public:
    explicit SslServerSocketWrapper(AddressFamily domain, const string &certPath, const string &keyPath);

    Response<bool> bindAndListen(const string& ip, int port, int backlog = 10);
    Response<SocketFD> accept();
    Response<size_t> send(SocketFD clientFd, const string& message);
    Response<string> receive(SocketFD clientFd);
    void close();

private:
    unique_ptr<SslServerSocket> serverSocket;
};
