#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    // Create the TCP server
    netflex::TcpServerSocket server(AddressFamily::IPv4);

    // Bind and start listening
    auto bindResult = server.bindAndListen("127.0.0.1", 8081);
    if (!bindResult.ok()) {
        cerr << "[TCP SERVER] Bind failed: " << bindResult.message << endl;
        return 1;
    }

    cout << "[TCP SERVER] Listening on port 8081..." << endl;

    // Accept a new connection
    auto acceptResult = server.acceptConnection();
    if (!acceptResult.ok()) {
        cerr << "[TCP SERVER] Accept failed: " << acceptResult.message << endl;
        return 1;
    }

    int clientFd = acceptResult.data.first;

    // Receive message from client
    auto recvResult = server.receive(clientFd);
    if (recvResult.ok()) {
        cout << "[TCP SERVER] Received: " << recvResult.data << endl;

        // Send response
        server.send(clientFd, "Hello from server!");
    } else {
        cerr << "[TCP SERVER] Receive failed: " << recvResult.message << endl;
    }

    server.closeSocket();

    return 0;
}
