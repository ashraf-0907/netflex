#pragma once

#include "../src/multithreaded-server/threaded-ssl-server-socket/threaded-ssl-server-socket.h"
#include <memory>

class ThreadedSslServerSocketWrapper
{
public:
    ThreadedSslServerSocketWrapper(AddressFamily family,
                             const string &certPath,
                             const string &keyPath,
                             size_t poolSize = 8,
                             size_t maxQueueSize = 1000);

    Response<bool> bindAndListen(const string &ip, int port, int backlog = 10);

    void setClientHandler(function<void(SocketFD, SslServerSocket *)> handler,
                          netflex::TaskPriority priority = netflex::TaskPriority::MEDIUM);

    void start();
    void stop();

private:
    unique_ptr<ThreadedSslServerSocket> server;
};