#include "./threaded-ssl-server-socket.h"
#include "../../../common/logger/logger.h"

using namespace netflex;

ThreadedSslServerSocket::ThreadedSslServerSocket(AddressFamily family, const string& certPath, const string& keyPath, size_t poolSize, size_t maxQueueSize):
serverSocket(make_unique<SslServerSocket>(family, certPath, keyPath)),
threadPool(make_unique<ThreadPool>(poolSize,maxQueueSize )),
clientTaskPriority(TaskPriority::MEDIUM), running(false){}

Response<bool> ThreadedSslServerSocket::bindAndListen(const string &ip, int port , int backlog){
    return serverSocket->bindAndListen(ip, port, backlog);
}

void ThreadedSslServerSocket::setClientHandler(std::function<void(SocketFD, SslServerSocket*)> handler, netflex::TaskPriority priority) {
    this->clientHandler = std::move(handler);
    this->clientTaskPriority = priority;
}

void ThreadedSslServerSocket::startAccepting(){
    running = true;
    thread(&ThreadedSslServerSocket::acceptLoop, this).detach();
}

void ThreadedSslServerSocket::stop(){
    this->running = false;
    serverSocket->closeSocket();
    threadPool->shutdown();
}

void ThreadedSslServerSocket::acceptLoop(){
    while(running){
        const auto clientResponse = serverSocket->acceptSslConnection();
        if(clientResponse.ok()){
            SocketFD clientFd = clientResponse.data;
            threadPool->enqueue([this, clientFd](){
                this->handleClient(clientFd);
            }, clientTaskPriority);
        }else {
            LOG_ERROR("SSL accept failed: " + clientResponse.message);
            continue;
        }
    }
}

void ThreadedSslServerSocket::handleClient(SocketFD clientFd) {
    if (clientHandler) {
        clientHandler(clientFd, serverSocket.get());
    } else {
        while (true) {
            auto recv = serverSocket->receiveData(clientFd);
            if (!recv.ok()) break;

            std::string msg = recv.data;
            serverSocket->sendData(clientFd, "Secure Echo: " + msg);
        }
        ::close(clientFd);
    }
}
