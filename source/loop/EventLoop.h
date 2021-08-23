#pragma once
#include "channel/squidChannel.h"
#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>
namespace squid
{
class EventLoop : public std::enable_shared_from_this<EventLoop>
{
  public:
    void RegisterEventHandler(std::shared_ptr<EventHandler> handler, int fd, bool enable = true);
    void Loop();
    void CreateEpollFd();
    void RunOnceInLoop(std::function<void()> func);
    void QueueToRunOnceInLoop(std::function<void()> func);
    EventLoop();
    ~EventLoop();

  private:
    std::unordered_map<int, std::shared_ptr<EventHandler>> eventHanderDict;
    int epollFd;
    std::vector<epoll_event> activeFdVec;
    void GetActiveFds(int num);
    void HandleActiveFds();
    std::vector<epoll_event> epollEventCollectVec;
    std::vector<std::function<void()>> _waitingFuncs;
    std::mutex _waitingFuncsLock;
    bool IsInLoopThread() const;
    void HandleWaitingFuncs();
    std::thread::id _threadId;
};
} // namespace squid