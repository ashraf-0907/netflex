#include "./threaded-ssl-server-socket-wrapper.h"

ThreadedSslServerSocketWrapper::ThreadedSslServerSocketWrapper(AddressFamily family, const string& certPath, const string& keyPath, size_t poolSize, size_t maxQueueSize) {
    server = make_unique<ThreadedSslServerSocket>(family, certPath, keyPath, poolSize, maxQueueSize);
}

Response<bool> ThreadedSslServerSocketWrapper::bindAndListen(const string& ip, int port, int backlog) {
    return server->bindAndListen(ip, port, backlog);
}

void ThreadedSslServerSocketWrapper::setClientHandler(function<void(SocketFD, SslServerSocket*)> handler, netflex::TaskPriority priority) {
    server->setClientHandler(handler, priority);
}

void ThreadedSslServerSocketWrapper::start() {
    server->startAccepting();
}

void ThreadedSslServerSocketWrapper::stop() {
    server->stop();
}

