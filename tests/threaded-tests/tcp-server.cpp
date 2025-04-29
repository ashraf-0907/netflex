#include "../../wrapper/threaded-tcp-server-socket-wrapper.h"
#include <iostream>

int main() {
    ThreadedTcpServerSocketWrapper server(AddressFamily::IPv4, 4, 100);

    server.setClientHandler(
        [](SocketFD fd, TcpServerSocket* socket) {
            std::string clientStr = std::to_string(fd);
            std::cout << "[TCP] Connected: FD=" << clientStr << std::endl;

            while (true) {
                auto recv = socket->receiveData(fd);
                if (!recv.ok()) {
                    std::cout << "[TCP] Client " << clientStr << " disconnected.\n";
                    break;
                }

                std::string msg = recv.data;
                socket->sendData(fd, "[Echo]: " + msg);
            }

            ::close(fd);
        },
        netflex::TaskPriority::MEDIUM
    );

    auto bindRes = server.bindAndListen("127.0.0.1", 9000);
    if (!bindRes.ok()) {
        std::cerr << "Bind failed: " << bindRes.message << std::endl;
        return 1;
    }

    std::cout << "[TCP] Server running on port 9000...\n";
    server.start();
    std::this_thread::sleep_for(std::chrono::minutes(10)); // Keep server alive
}
