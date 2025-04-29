#include "ssl-server-wrapper.h"

SslServerSocketWrapper::SslServerSocketWrapper(AddressFamily domain,const string &certPath,const string &keyPath) {
    serverSocket = make_unique<SslServerSocket>(domain, certPath, keyPath);
}

Response<bool> SslServerSocketWrapper::bindAndListen(const string& ip, int port, int backlog) {
    return serverSocket->bindAndListen(ip, port, backlog);
}

Response<SocketFD> SslServerSocketWrapper::accept() {
    return serverSocket->acceptSslConnection();
}

Response<size_t> SslServerSocketWrapper::send(SocketFD clientFd, const string& message) {
    return serverSocket->sendData(clientFd, message);
}

Response<string> SslServerSocketWrapper::receive(SocketFD clientFd) {
    return serverSocket->receiveData(clientFd);
}

void SslServerSocketWrapper::close() {
    serverSocket->closeSocket();
}
