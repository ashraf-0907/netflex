/**
 * ---------------------------------------------------------------------
 * @section NetFlex - ThreadedTcpServerSocket Design Summary
 * ---------------------------------------------------------------------
 *
 * 🔹 Purpose:
 * This class implements a multithreaded TCP server using thread.
 * It allows injecting custom per-client handling logic at runtime.
 *
 * 🔹 Key Concepts:
 *  - Accepts incoming client connections using TcpServerSocket
 *  - Spawns a new thread for each client connection (1:1 model)
 *  - Uses function (lambda/fn) to inject logic dynamically
 *  - Enables flexible and reusable server-side behavior
 *
 * 🔹 Lambda Handler Design:
 * We define a handler type:
 *     using TcpClientHandler = function<void(SocketFD, TcpServerSocket*)>;
 *
 * Then allow the user to inject logic:
 *     threadedServer.setClientHandler(myHandler);
 *
 * Internally, we call:
 *     clientHandler(clientFd, serverSocket);
 * from a dedicated thread per client.
 *
 * 🔹 Lambda Example (Injected from Wrapper or App):
 *     [](SocketFD fd, TcpServerSocket* socket) {
 *         auto msg = socket->receiveData(fd);
 *         socket->sendData(fd, "Echo: " + msg.data);
 *     };
 *
 * 🔹 Benefits:
 * Logic is cleanly separated from socket and threading mechanics
 * Server can support any use case (chat, auth, telemetry, etc.)
 * Easy to extend, test, and reuse
 * Supports both custom and fallback behavior
 *
 * 🔹 Maintainer Notes:
 * - `this` must be captured in lambdas to call member methods
 * - Detaching threads is fine for basic server use; for controlled scaling, use ThreadPool
 * - Ensure graceful cleanup in production (join threads or use atomic flags)
 *
 * ---------------------------------------------------------------------
 */

#include "./threaded-tcp-server-socket.h"

ThreadedTcpServerSocket::ThreadedTcpServerSocket(AddressFamily family, size_t poolSize, size_t maxQueue)
    : serverSocket(make_unique<TcpServerSocket>(family)),
      threadPool(make_unique<ThreadPool>(poolSize, maxQueue)),
      running(false) {}

Response<bool> ThreadedTcpServerSocket::bindAndListen(const string &ip, int port, int backlog){
    return serverSocket->bindAndListen(ip, port , backlog);
}

void ThreadedTcpServerSocket::setClientHandler(std::function<void(SocketFD, TcpServerSocket *)> handler, netflex::TaskPriority priority)
{
    this->clientHandler = std::move(handler);
    this->clientTaskPriority = priority;
}

void ThreadedTcpServerSocket::startAccepting()
{
    running = true;
    thread(&ThreadedTcpServerSocket::acceptLoop, this).detach();
}

void ThreadedTcpServerSocket::stop()
{
    running = false;
    threadPool->shutdown();
}

void ThreadedTcpServerSocket::acceptLoop()
{
    while (running)
    {
        auto clientResponse = serverSocket->acceptConnection();
        if (clientResponse.ok())
        {
            SocketFD clientFd = clientResponse.data.first;

            threadPool->enqueue([this, clientFd]() {
                this->handleClient(clientFd);
            }, clientTaskPriority);
        }
    }
}

void ThreadedTcpServerSocket::handleClient(SocketFD clientFd)
{
    if (clientHandler)
    {
        clientHandler(clientFd, serverSocket.get());
    }
    else
    {
        while (true)
        {
            auto res = serverSocket->receiveData(clientFd);
            if (!res.ok())
                break;

            auto msg = res.data;
            serverSocket->sendData(clientFd, "Echo: " + msg);
        }
        ::close(clientFd);
    }
}
