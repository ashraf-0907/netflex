#include "./tcp-server-wrapper.h"

TcpServerSocketWrapper::TcpServerSocketWrapper(AddressFamily domain) {
    serverSocket = make_unique<TcpServerSocket>(domain);
}

Response<bool> TcpServerSocketWrapper::bindAndListen(const string& host, int port, int backlog) {
    return serverSocket->bindAndListen(host, port, backlog);
}

Response<pair<int, sockaddr_storage>> TcpServerSocketWrapper::acceptConnection() {
    return serverSocket->acceptConnection();
}

Response<string> TcpServerSocketWrapper::receive(int clientFd) {
    return serverSocket->receiveData(clientFd);
}

Response<size_t> TcpServerSocketWrapper::send(int clientFd, const string &message) {
    return serverSocket->sendData(clientFd, message);
}

void TcpServerSocketWrapper::closeSocket() {
    serverSocket->closeSocket();
}

int TcpServerSocketWrapper::getSocketFd() const {
    return serverSocket->getSocketFd();
}
