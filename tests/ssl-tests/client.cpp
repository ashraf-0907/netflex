#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    netflex::SslClientSocket client(AddressFamily::IPv4, "127.0.0.1", 8443);

    auto sendRes = client.sendMessage("Hello SSL Server!");
    if (!sendRes.ok()) {
        cerr << "[SSL CLIENT] Send failed: " << sendRes.message << endl;
        return 1;
    }
    cout << "[SSL CLIENT] Message sent." << endl;

    auto recvRes = client.receiveMessage();
    if (recvRes.ok()) {
        cout << "[SSL CLIENT] Received: " << recvRes.data << endl;
    } else {
        cerr << "[SSL CLIENT] Receive failed: " << recvRes.message << endl;
    }

    client.close();
    return 0;
}
