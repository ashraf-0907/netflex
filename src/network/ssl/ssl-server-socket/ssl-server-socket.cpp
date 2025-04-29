#include "ssl-server-socket.h"
#include "../../../../common/logger/logger.h"

SslServerSocket::SslServerSocket(AddressFamily domain,const string &certPath,const string &keyPath)
    : TcpServerSocket(domain),
      ioHelper(make_unique<SocketIOHelper>()),
      certPath(certPath),
      keyPath(keyPath) {}

SslServerSocket::~SslServerSocket() {
    closeSocket();
}

Response<bool> SslServerSocket::bindAndListen(const string& ip, int port, int backlog) {
    return TcpServerSocket::bindAndListen(ip, port, backlog);
}

Response<SocketFD> SslServerSocket::acceptSslConnection() {
    auto tcpRes = TcpServerSocket::acceptConnection();
    if (!tcpRes.ok()) {
        LOG_ERROR("TCP accept failed: " + tcpRes.message);
        return Response<SocketFD>::error(SocketStatus::CONNECTION_FAILED, tcpRes.message);
    }

    auto [clientFd, clientAddr] = tcpRes.data;
    auto sslSock = make_unique<SslSocket>(true, certPath, keyPath);

    auto initRes = sslSock->initializeContext();
    if (!initRes.ok()) {
        LOG_ERROR("SSL context init failed for client fd " + to_string(clientFd));
        ::close(clientFd);
        return Response<SocketFD>::error(SocketStatus::UNKNOWN_ERROR, initRes.message);
    }

    auto attachRes = sslSock->attachToSocket(clientFd);
    if (!attachRes.ok()) {
        LOG_ERROR("SSL attach failed for client fd " + to_string(clientFd));
        ::close(clientFd);
        return Response<SocketFD>::error(SocketStatus::UNKNOWN_ERROR, attachRes.message);
    }

    auto handshakeRes = sslSock->performHandshake();
    if (!handshakeRes.ok()) {
        LOG_ERROR("SSL handshake failed for client fd " + to_string(clientFd));
        ::close(clientFd);
        return Response<SocketFD>::error(SocketStatus::HANDSHAKE_FAILED, handshakeRes.message);
    }

    clientSslMap[clientFd] = move(sslSock);
    return Response<SocketFD>::success(clientFd);
}

Response<size_t> SslServerSocket::sendData(SocketFD clientFd, const string& message) {
    if (!clientSslMap.count(clientFd)) {
        return Response<size_t>::error(SocketStatus::INVALID_ARGUMENT, "Client not found");
    }
    return ioHelper->sendFramedSSL(clientSslMap[clientFd].get(), message);
}

Response<string> SslServerSocket::receiveData(SocketFD clientFd) {
    if (!clientSslMap.count(clientFd)) {
        return Response<string>::error(SocketStatus::INVALID_ARGUMENT, "Client not found");
    }
    return ioHelper->recvFramedSSL(clientSslMap[clientFd].get());
}

void SslServerSocket::closeSocket() {
    for (auto& [fd, sslSock] : clientSslMap) {
        sslSock->shutdown();
    }
    clientSslMap.clear();
    TcpServerSocket::closeSocket();
}
