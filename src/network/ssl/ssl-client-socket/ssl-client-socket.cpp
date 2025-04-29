#include "ssl-client-socket.h"
#include "../../../../common/logger/logger.h"

SslClientSocket::SslClientSocket(AddressFamily family)
    : TcpClientSocket(family),
      sslSocket(make_unique<SslSocket>(false)),
      ioHelper(make_unique<SocketIOHelper>()) {
    sslSocket->initializeContext();  
}

SslClientSocket::~SslClientSocket() {
    closeSocket();
}

Response<bool> SslClientSocket::connectToServer(const string& host, int port) {
    auto tcpRes = TcpClientSocket::connectToServer(host, port);
    if (!tcpRes.ok()) {
        LOG_ERROR("TCP connect failed: " + tcpRes.message);
        return tcpRes;
    }

    auto attachRes = sslSocket->attachToSocket(getSocketFd());
    if (!attachRes.ok()) {
        LOG_ERROR("SSL attach failed: " + attachRes.message);
        return attachRes;
    }

    auto handshake = sslSocket->performHandshake();
    if (!handshake.ok()) {
        LOG_ERROR("SSL handshake failed: " + handshake.message);
        return handshake;
    }

    LOG_INFO("SSL client handshake complete");
    return Response<bool>::success(true);
}

Response<size_t> SslClientSocket::sendData(const string& message) {
    return ioHelper->sendFramedSSL(sslSocket.get(), message);
}

Response<string> SslClientSocket::receiveData() {
    return ioHelper->recvFramedSSL(sslSocket.get());
}

void SslClientSocket::closeSocket() {
    sslSocket->shutdown();
    TcpClientSocket::closeSocket();
}