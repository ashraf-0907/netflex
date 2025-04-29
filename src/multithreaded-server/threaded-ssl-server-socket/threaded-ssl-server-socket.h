#include "../../../common/utils/socket-defines.h"
#include "../../../common/utils/net-utils/net-utils.h"
#include "../../network/ssl/ssl-server-socket/ssl-server-socket.h"
#include "../../../common/task/task.h"
#include "../thread-pool/thread-pool.h"

class ThreadedSslServerSocket {
    public:
        ThreadedSslServerSocket(AddressFamily family, const string& certPath, const string& keyPath, size_t poolSize = 8, size_t maxQueue = 1000);
    
        void setClientHandler(function<void(SocketFD, SslServerSocket*)> handler, TaskPriority priority = TaskPriority::MEDIUM);
    
        Response<bool> bindAndListen(const string& ip, int port, int backlog = 10);
        void startAccepting();
        void stop();
    
    private:
        unique_ptr<SslServerSocket> serverSocket;
        unique_ptr<ThreadPool> threadPool;
        function<void(SocketFD, SslServerSocket*)> clientHandler;
        TaskPriority clientTaskPriority;
    
        atomic<bool> running;
        void acceptLoop();
        void handleClient(SocketFD clientFd);
    };
    