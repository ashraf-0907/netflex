#pragma once

#include "../../../common/response/response.h"
#include <openssl/ssl.h>
#include <string>
#include "../../../common/logger/logger.h"
#include <openssl/err.h>
#include <unistd.h>
#include "../../../common/utils/socket-defines.h"

class SslSocket {
public:
    SslSocket(bool isServer = false, const std::string& certPath = "", const std::string& keyPath = "");
    ~SslSocket();

    Response<bool> initializeContext();
    Response<bool> attachToSocket(SocketFD fd);
    Response<bool> performHandshake();

    int sslWrite(const char* data, size_t length);
    int sslRead(char* buffer, size_t bufferSize);

    void shutdown();

private:
    SSL_CTX* sslCtx;
    SSL* ssl;
    SocketFD attachedFd;
    bool isServer;

    std::string certPath;
    std::string keyPath;

    bool loadCertificates(); // Only required for server
};
