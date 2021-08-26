#include "EventLoop.h"
#include "error/errorUtility.h"
#include <mutex>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <thread>
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
EventLoop::EventLoop() : epollEventCollectVec(100), epollFd(-1), _threadId(std::this_thread::get_id())
{
    CreateEpollFd();
}
EventLoop::~EventLoop()
{
    CommonUtility::CloseFd(epollFd);
}
void EventLoop::GetActiveFds(int num)
{
    activeFdVec.clear();
    auto size = epollEventCollectVec.size();
    for (int i = 0; i < num; i++)
    {
        auto &it = epollEventCollectVec[i];
        activeFdVec.emplace_back(std::move(it));
    }
    if (num == size)
    {
        epollEventCollectVec.resize(num * 3 / 2);
    }
}

void EventLoop::HandleActiveFds()
{
    for (auto it : activeFdVec)
    {
        auto fd = it.data.fd;
        auto type = it.events;
        if (auto it2 = eventHanderDict.find(fd); it2 != eventHanderDict.end())
        {
            auto &handler = it2->second;
            if ((type & POLLHUP) && !(type & POLLIN))
            {
                handler->Handle(EventType::Close, fd);
            }
            if ((type & (POLLERR | POLLNVAL)))
            {
                handler->Handle(EventType::Error, fd);
            }
            if ((type & (POLLIN | POLLPRI | POLLRDHUP)))
            {
                handler->Handle(EventType::Read, fd);
            }
            if ((type & (POLLOUT)))
            {
                handler->Handle(EventType::Write, fd);
            }
        }
    }
}
void EventLoop::Loop()
{
    while (true)
    {
        if (auto num = epoll_wait(epollFd, &*epollEventCollectVec.begin(), epollEventCollectVec.size(), 10); num > 0)
        {
            GetActiveFds(num);
        }
        HandleActiveFds();
        HandleWaitingFuncs();
    }
}

void EventLoop::HandleWaitingFuncs()
{
    std::unique_lock lock(_waitingFuncsLock);
    for (auto &it : _waitingFuncs)
    {
        it();
    }
}
void EventLoop::UpdateEventHandlerByFd(int fd)
{
    if (auto it = eventHanderDict.find(fd); it != eventHanderDict.end())
    {
        struct epoll_event event;
        bzero(&event, sizeof(event));
        event.data.fd = fd;
        event.events |= it->second->GetEnabledEventType();

        if (auto res = epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event); res == -1)
        {
            ErrorUtility::LogError(SocketError::EpollAdd);
            return;
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
        opt = eventHanderDict.count(fd) > 0 ? EPOLL_CTL_DEL : 0;

        eventHanderDict.erase(fd);
    }
    else
    {
        opt = eventHanderDict.count(fd) > 0 ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
        eventHanderDict[fd] = handler;
    }
    event.events |= handler->GetEnabledEventType();
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

void EventLoop::RunOnceInLoop(std::function<void()> func)
{
    if (IsInLoopThread())
    {
        func();
    }
    else
    {
        QueueToRunOnceInLoop(std::move(func));
    }
}

void EventLoop::QueueToRunOnceInLoop(std::function<void()> func)
{
    std::unique_lock lock(_waitingFuncsLock);
    _waitingFuncs.emplace_back(std::move(func));
}

bool EventLoop::IsInLoopThread() const
{
    // return true;
    return _threadId == std::this_thread::get_id();
}
