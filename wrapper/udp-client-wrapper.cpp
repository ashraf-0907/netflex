#include "udp-client-wrapper.h"

UdpClientSocketWrapper::UdpClientSocketWrapper(AddressFamily domain) {
    udpClient = std::make_unique<UdpClientSocket>(domain);
}

Response<size_t> UdpClientSocketWrapper::sendMessage(const std::string& message, const std::string& ip, int port) {
    return udpClient->sendTo(message, ip, port);
}

Response<std::pair<std::string, ServerAddressStruct>> UdpClientSocketWrapper::receiveMessage() {
    return udpClient->receiveFrom();
}

void UdpClientSocketWrapper::close() {
    udpClient->closeSocket();
}
