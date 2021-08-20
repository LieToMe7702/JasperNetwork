#include "EventLoop.h"
#include "error/errorUtility.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <utility/squidUtility.h>
using namespace squid;
void EventLoop::CreateEpollFd()
{
    if (epollFd = epoll_create1(EPOLL_CLOEXEC); epollFd == -1)
    {
        ErrorUtility::LogError(SocketError::EpollCreate);
        return;
    }
}
EventLoop::EventLoop() : epollEventCollectVec(100)
{
    CreateEpollFd();
}
EventLoop::~EventLoop()
{
    Utility::CloseFd(epollFd);
}
void EventLoop::GetActiveFds(int num)
{
    // activeFdVec.clear();
    for (auto &it : epollEventCollectVec)
    {
        // activeFdVec.push_back(it.data.fd);
        auto fd = it.data.fd;
        if (auto it2 = eventHanderDict.find(fd); it2 != eventHanderDict.end())
        {
            it2->second->Handle(it.events, fd);
        }
    }
    if (num == epollEventCollectVec.size())
    {
        epollEventCollectVec.resize(num * 3 / 2);
    }
}

void EventLoop::HandleActiveFds()
{
}
void EventLoop::Loop()
{

    struct sockaddr_in clientAddr;
    auto size = sizeof(clientAddr);
    char buf[1000];
    while (true)
    {
        if (auto num = epoll_wait(epollFd, &*epollEventCollectVec.begin(), epollEventCollectVec.size(), 10); num > 0)
        {
            GetActiveFds(num);
            // HandleActiveFds();
            /*
            size = sizeof(clientAddr);
            auto connFd = accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr),
                                 reinterpret_cast<socklen_t *>(&size));
            std::cout << connFd << std::endl;
            close(connFd);*/
        }
    }
}
void EventLoop::RegisterEventHandler(std::shared_ptr<EventHandler> handler, int fd, bool enable)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.fd = fd;
    int opt = 0;
    if (!enable)
    {
        opt = eventHanderDict.contains(fd) ? EPOLL_CTL_DEL : 0;

        eventHanderDict.erase(fd);
    }
    else
    {
        opt = eventHanderDict.contains(fd) ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
        eventHanderDict[fd] = handler;
    }
    event.events = static_cast<uint32_t>(EventType::Read);
    if (opt == 0)
    {
        return;
    }
    if (auto res = epoll_ctl(epollFd, opt, fd, &event); res == -1)
    {
        ErrorUtility::LogError(SocketError::EpollAdd);
        return;
    }
}