#include "../../include/netflex/netflex.h"
#include <iostream>

int main() {
    netflex::SslServerSocket server(AddressFamily::IPv4, "crets/cert.pem", "crets/key.pem");

    auto bindRes = server.bindAndListen("127.0.0.1", 8443);
    if (!bindRes.ok()) {
        cerr << "[SSL SERVER] Bind failed: " << bindRes.message << endl;
        return 1;
    }

    cout << "[SSL SERVER] Listening on 127.0.0.1:8443" << endl;

    auto acceptRes = server.accept();
    if (!acceptRes.ok()) {
        cerr << "[SSL SERVER] Accept failed: " << acceptRes.message << endl;
        return 1;
    }

    int clientFd = acceptRes.data;

    auto recvRes = server.receive(clientFd);
    if (recvRes.ok()) {
        cout << "[SSL SERVER] Received: " << recvRes.data << endl;
        server.send(clientFd, "Hello from SSL Server!");
    } else {
        cerr << "[SSL SERVER] Receive failed: " << recvRes.message << endl;
    }

    server.close();
    return 0;
}
