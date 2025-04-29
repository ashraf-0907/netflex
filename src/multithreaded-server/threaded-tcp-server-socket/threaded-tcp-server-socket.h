#pragma once

#include "../thread-pool/thread-pool.h"  // or wherever your ThreadPool lives
#include "../../../common/utils/net-utils/net-utils.h"
#include "../../../common/utils/socket-defines.h"
#include "../../network/tcp/tcp-server-socket/tcp-server-socket.h"

using namespace netflex;


class ThreadedTcpServerSocket {
public:
    ThreadedTcpServerSocket(AddressFamily family, size_t poolSize = 8, size_t maxQueue = 1000);
    Response<bool> bindAndListen(const string &ip, int port, int backlog);
    void setClientHandler(function<void(SocketFD, TcpServerSocket*)> handler, TaskPriority priority = TaskPriority::MEDIUM);
    void startAccepting();
    void stop();

private:
    unique_ptr<TcpServerSocket> serverSocket;
    unique_ptr<ThreadPool> threadPool;
    function<void(SocketFD, TcpServerSocket*)> clientHandler;
    atomic<bool> running;
    TaskPriority clientTaskPriority;

    void acceptLoop();
    void handleClient(SocketFD clientFd);
};
