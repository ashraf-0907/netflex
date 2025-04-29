#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    netflex::UdpClientSocket client(AddressFamily::IPv4);

    string serverIp = "127.0.0.1";
    int serverPort = 8082;

    auto sendResult = client.sendMessage("Hello from UDP Client!", serverIp, serverPort);
    if (!sendResult.ok()) {
        cerr << "[UDP CLIENT] Send failed: " << sendResult.message << endl;
        return 1;
    }

    cout << "[UDP CLIENT] Message sent.\n";

    auto recvResult = client.receiveMessage();
    if (recvResult.ok()) {
        cout << "[UDP CLIENT] Received: " << recvResult.data.first << endl;
    } else {
        cerr << "[UDP CLIENT] Receive failed: " << recvResult.message << endl;
    }

    return 0;
}
