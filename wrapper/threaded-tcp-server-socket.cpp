#include "./threaded-tcp-server-socket-wrapper.h"

ThreadedTcpServerSocketWrapper::ThreadedTcpServerSocketWrapper(AddressFamily family, size_t poolSize, size_t maxQueueSize) {
    server = std::make_unique<ThreadedTcpServerSocket>(family, poolSize, maxQueueSize);
}

Response<bool> ThreadedTcpServerSocketWrapper::bindAndListen(const std::string& ip, int port, int backlog) {
    return server->bindAndListen(ip, port, backlog);
}

void ThreadedTcpServerSocketWrapper::setClientHandler(std::function<void(SocketFD, TcpServerSocket*)> handler, netflex::TaskPriority priority) {
    server->setClientHandler(handler, priority);
}

void ThreadedTcpServerSocketWrapper::start() {
    server->startAccepting();
}

void ThreadedTcpServerSocketWrapper::stop() {
    server->stop();
}
