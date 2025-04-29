#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    netflex::UdpServerSocket server(AddressFamily::IPv4);

    // Bind to localhost at port 8082
    auto bindResult = server.bindToAddress("127.0.0.1", 8082);
    if (!bindResult.ok()) {
        cerr << "[UDP SERVER] Bind failed: " << bindResult.message << endl;
        return 1;
    }

    cout << "[UDP SERVER] Waiting for message...\n";

    auto recvResult = server.receiveFrom();
    if (recvResult.ok()) {
        string message = recvResult.data.first;
        ServerAddressStruct clientAddr = recvResult.data.second;

        cout << "[UDP SERVER] Received: " << message << endl;

        auto sendResult = server.sendTo("Hello from UDP Server!", clientAddr);
        if (!sendResult.ok()) {
            cerr << "[UDP SERVER] Send failed: " << sendResult.message << endl;
        }
    } else {
        cerr << "[UDP SERVER] Receive failed: " << recvResult.message << endl;
    }

    return 0;
}
