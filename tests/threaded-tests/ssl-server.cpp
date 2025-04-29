#include "../../wrapper/threaded-ssl-server-socket-wrapper.h"
#include <iostream>

int main() {
    ThreadedSslServerSocketWrapper server(AddressFamily::IPv4,
                                    "certs/server-cert.pem",
                                    "certs/server-key.pem",
                                    4, 100);

    server.setClientHandler(
        [](SocketFD fd, SslServerSocket* socket) {
            std::cout << "[SSL] Secure client connected: FD=" << fd << std::endl;

            while (true) {
                auto recv = socket->receiveData(fd);
                if (!recv.ok()) {
                    std::cout << "[SSL] Client disconnected.\n";
                    break;
                }

                std::string msg = recv.data;
                socket->sendData(fd, "[Secure Echo]: " + msg);
            }

            ::close(fd);
        },
        netflex::TaskPriority::HIGH
    );

    auto bindRes = server.bindAndListen("127.0.0.1", 9443);
    if (!bindRes.ok()) {
        std::cerr << "SSL Bind failed: " << bindRes.message << std::endl;
        return 1;
    }

    std::cout << "[SSL] Secure server running on port 9443...\n";
    server.start();
    std::this_thread::sleep_for(std::chrono::minutes(10));
}
