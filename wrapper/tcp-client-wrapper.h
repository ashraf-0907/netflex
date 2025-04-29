#pragma once

#include "../src/network/tcp/tcp-client-socket/tcp-client-socket.h"
#include <string>

using namespace std;

class TcpClientSocketWrapper {
public:
    TcpClientSocketWrapper(AddressFamily family);
    bool connectToServer(const string& ip, int port);
    bool sendMessage(const string& message);
    string receiveMessage();
    void closeConnection();

private:
    TcpClientSocket clientSocket;
};
