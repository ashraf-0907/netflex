#include "ssl-socket.h"

SslSocket::SslSocket(bool isServer, const std::string& certPath, const std::string& keyPath)
    : sslCtx(nullptr), ssl(nullptr), attachedFd(-1), isServer(isServer),
      certPath(certPath), keyPath(keyPath) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    LOG_DEBUG("OpenSSL initialized");
}

SslSocket::~SslSocket() {
    shutdown();
}

Response<bool> SslSocket::initializeContext() {
    const SSL_METHOD* method = isServer ? TLS_server_method() : TLS_client_method();
    sslCtx = SSL_CTX_new(method);

    if (!sslCtx) {
        LOG_ERROR("Failed to create SSL_CTX");
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "SSL_CTX_new failed");
    }

    if (isServer && !loadCertificates()) {
        LOG_ERROR("Certificate loading failed");
        return Response<bool>::error(SocketStatus::CERTIFICATE_ERROR, "Server certificates not loaded");
    }

    return Response<bool>::success(true);
}

bool SslSocket::loadCertificates() {
    if (SSL_CTX_use_certificate_file(sslCtx, certPath.c_str(), SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(sslCtx, keyPath.c_str(), SSL_FILETYPE_PEM) <= 0 ||
        !SSL_CTX_check_private_key(sslCtx)) {
        LOG_ERROR("Invalid certificate or key for SSL server");
        return false;
    }

    return true;
}

Response<bool> SslSocket::attachToSocket(SocketFD fd) {
    if (!sslCtx) {
        auto ctxRes = initializeContext();
        if (!ctxRes.ok()) return ctxRes;
    }

    ssl = SSL_new(sslCtx);
    if (!ssl) return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "SSL_new failed");

    SSL_set_fd(ssl, fd);
    attachedFd = fd;
    return Response<bool>::success(true);
}

Response<bool> SslSocket::performHandshake() {
    int res = isServer ? SSL_accept(ssl) : SSL_connect(ssl);
    if (res <= 0) {
        LOG_ERROR("SSL handshake failed");
        return Response<bool>::error(SocketStatus::HANDSHAKE_FAILED, "SSL handshake failed");
    }
    return Response<bool>::success(true);
}

int SslSocket::sslWrite(const char* data, size_t length) {
    return SSL_write(ssl, data, length);
}

int SslSocket::sslRead(char* buffer, size_t bufferSize) {
    return SSL_read(ssl, buffer, bufferSize);
}

void SslSocket::shutdown() {
    if (ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        ssl = nullptr;
    }

    if (sslCtx) {
        SSL_CTX_free(sslCtx);
        sslCtx = nullptr;
    }

    if (attachedFd >= 0) {
        close(attachedFd);
        attachedFd = -1;
    }
}
