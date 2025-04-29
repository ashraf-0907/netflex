#include "ssl-client-wrapper.h"

SslClientSocketWrapper::SslClientSocketWrapper(AddressFamily domain, const string& serverIp, int port) {
    sslClient = make_unique<SslClientSocket>(domain);

    auto result = sslClient->connectToServer(serverIp, port);
    if (!result.ok()) {
        throw runtime_error("Failed to connect to SSL server: " + result.message);
    }
}

Response<size_t> SslClientSocketWrapper::sendMessage(const string& message) {
    return sslClient->sendData(message);
}

Response<string> SslClientSocketWrapper::receiveMessage() {
    return sslClient->receiveData();
}

void SslClientSocketWrapper::close() {
    sslClient->closeSocket();
}
