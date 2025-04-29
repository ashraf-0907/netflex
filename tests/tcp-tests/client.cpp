#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    netflex::TcpClientSocket *client = new TcpClientSocketWrapper(AddressFamily::IPv4);

    auto connectResult = client->connectToServer("127.0.0.1", 8081);  // Should fail if server isn't running
    if (connectResult) {
        std::cout << "[TCP CLIENT] Connected!" << std::endl;
        auto sendResult = client->sendMessage("Hello Server!");
        std::cout << "[TCP CLIENT] Sent status: " << sendResult << std::endl;

        auto recvResult = client->receiveMessage();
        std::cout << "[TCP CLIENT] Received: " << recvResult<< std::endl;

        client->closeConnection();
    } else {
        std::cout << "[TCP CLIENT] Failed to connect: " << connectResult << std::endl;
    }

    return 0;
}
