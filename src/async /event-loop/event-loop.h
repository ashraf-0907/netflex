#pragma once

#include <functional>
#include "../../common/utils/socket-defines.h"
#include "../../common/utils/net-utils/net-utils.h"
#include "../../common/response/response.h"

using namespace std;

enum class EventType
{
    READ,
    WRITE
};

class EventLoop
{
public:
    using CallBack = function<void(int)>;

    virtual Response<bool> addSocketFD(SocketFD socketFd, EventType type, CallBack function) = 0;
    virtual Response<bool> removeSocketFD(SocketFD socketFd) = 0;
    virtual Response<bool> modifySocketFD(SocketFD socketFd, EventType newType) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual ~EventLoop() = default;
};