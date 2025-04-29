#pragma once

#include "../src/multithreaded-server/threaded-tcp-server-socket/threaded-tcp-server-socket.h"
#include <memory>

class ThreadedTcpServerSocketWrapper {
public:
    ThreadedTcpServerSocketWrapper(AddressFamily family,
                             size_t poolSize = 8,
                             size_t maxQueueSize = 1000);

    Response<bool> bindAndListen(const std::string& ip, int port, int backlog = 10);

    void setClientHandler(std::function<void(SocketFD, TcpServerSocket*)> handler,
                          netflex::TaskPriority priority = netflex::TaskPriority::MEDIUM);

    void start();
    void stop();

private:
    std::unique_ptr<ThreadedTcpServerSocket> server;
};
