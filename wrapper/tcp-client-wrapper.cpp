#include "tcp-client-wrapper.h"

TcpClientSocketWrapper::TcpClientSocketWrapper(AddressFamily family):clientSocket(family){ }

bool TcpClientSocketWrapper::connectToServer(const string& ip, int port) {
    return clientSocket.connectToServer(ip, port).ok();
}

bool TcpClientSocketWrapper::sendMessage(const string& message) {
    return clientSocket.sendData(message).ok();
}

string TcpClientSocketWrapper::receiveMessage() {
    auto response = clientSocket.receiveData();
    return response.ok() ? response.data : "";
}

void TcpClientSocketWrapper::closeConnection() {
    clientSocket.closeSocket();
}
