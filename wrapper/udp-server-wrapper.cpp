#include "udp-server-wrapper.h"

UdpServerSocketWrapper::UdpServerSocketWrapper(AddressFamily domain) {
    server = make_unique<UdpServerSocket>(domain);
}

UdpServerSocketWrapper::~UdpServerSocketWrapper() = default;

Response<bool> UdpServerSocketWrapper::bindToAddress(const string &ip, int port){
    return server->bindToAddress(ip, port);
}

Response<pair<string, ServerAddressStruct>> UdpServerSocketWrapper::receiveFrom() {
    return server->receiveFrom();
}

Response<size_t> UdpServerSocketWrapper::sendTo(const string& message, const ServerAddressStruct &clientAddr) {
    return server->sendTo(message, clientAddr);
}
