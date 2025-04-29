#pragma once

#include "../event-loop.h"
#include <map>

class EpollEventLoop : public EventLoop
{
public:
    EpollEventLoop();
    ~EpollEventLoop();

    Response<bool> addSocketFD(SocketFD socketFd, EventType type, CallBack callback) override;
    Response<bool> removeSocketFD(SocketFD socketFd) override;
    Response<bool> modifySocketFD(SocketFD socketFd, EventType type) override;
    void run() override;
    void stop() override;

private:
    int epollFd;
    bool running;
    unordered_map<SocketFD, CallBack> socketFdCallBackMap;
};