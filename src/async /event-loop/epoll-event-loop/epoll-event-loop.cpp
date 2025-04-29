#include "epoll-event-loop.h"
#include "../../../../common/logger/logger.h"


#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

EpollEventLoop::EpollEventLoop()
{
    epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        LOG_ERROR("Failed to create epoll instance: " + std::string(strerror(errno)));
        throw std::runtime_error("epoll_create1 failed");
    }
    running = false;
    LOG_INFO("EpollEventLoop initialized with epollFd: " + std::to_string(epollFd));
}

EpollEventLoop::~EpollEventLoop()
{
    close(epollFd);
    LOG_INFO("EpollEventLoop destroyed, epollFd closed.");
}

Response<bool> EpollEventLoop::addSocketFD(SocketFD socketFd, EventType type, CallBack callback)
{
    epoll_event ev = {};
    ev.data.fd = socketFd;
    ev.events = (type == EventType::READ) ? EPOLLIN : EPOLLOUT;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &ev) == -1)
    {
        LOG_ERROR("epoll_ctl ADD failed: " + std::string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Failed to add FD to epoll");
    }

    socketFdCallBackMap[socketFd] = callback;
    LOG_INFO("FD " + std::to_string(socketFd) + " added to epoll.");
    return Response<bool>::success(true);
}

Response<bool> EpollEventLoop::removeSocketFD(SocketFD socketFd)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, socketFd, nullptr) == -1)
    {
        LOG_ERROR("epoll_ctl DEL failed: " + std::string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Failed to remove FD from epoll");
    }

    socketFdCallBackMap.erase(socketFd);
    LOG_INFO("FD " + std::to_string(socketFd) + " removed from epoll.");
    return Response<bool>::success(true);
}

Response<bool> EpollEventLoop::modifySocketFD(SocketFD socketFd, EventType type)
{
    epoll_event ev = {};
    ev.data.fd = socketFd;
    ev.events = (type == EventType::READ) ? EPOLLIN : EPOLLOUT;

    if (epoll_ctl(epollFd, EPOLL_CTL_MOD, socketFd, &ev) == -1)
    {
        LOG_ERROR("epoll_ctl MOD failed: " + std::string(strerror(errno)));
        return Response<bool>::error(SocketStatus::UNKNOWN_ERROR, "Failed to modify FD in epoll");
    }

    LOG_DEBUG("FD " + std::to_string(socketFd) + " modified in epoll.");
    return Response<bool>::success(true);
}

void EpollEventLoop::run()
{
    running = true;
    constexpr int MAX_EVENTS = 64;
    epoll_event events[MAX_EVENTS];

    LOG_INFO("EpollEventLoop run loop started.");

    while (running)
    {
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            if (errno == EINTR)
                continue;
            LOG_ERROR("epoll_wait failed: " + std::string(strerror(errno)));
            break;
        }

        for (int i = 0; i < nfds; ++i)
        {
            int fd = events[i].data.fd;
            auto it = socketFdCallBackMap.find(fd);
            if (it != socketFdCallBackMap.end())
            {
                LOG_DEBUG("Triggering callback for FD: " + std::to_string(fd));
                it->second(fd);
            }
            else
            {
                LOG_WARNING("No callback found for FD: " + std::to_string(fd));
            }
        }
    }

    LOG_INFO("EpollEventLoop run loop exited.");
}

void EpollEventLoop::stop()
{
    running = false;
    LOG_INFO("EpollEventLoop marked to stop.");
}
