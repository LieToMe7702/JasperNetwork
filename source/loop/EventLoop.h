#pragma once
#include "channel/squidChannel.h"
#include <memory>
#include <unordered_map>
#include <vector>
namespace squid
{
class EventLoop
{
  public:
    void RegisterEventHandler(std::shared_ptr<EventHandler> handler, int fd, bool enable = true);
    void Loop();
    void CreateEpollFd();

    EventLoop();
    ~EventLoop();

  private:
    std::unordered_map<int, std::shared_ptr<EventHandler>> eventHanderDict;
    int epollFd;
    std::vector<int> activeFdVec;
    void GetActiveFds(int num);
    void HandleActiveFds();
    std::vector<epoll_event> epollEventCollectVec;
};
} // namespace squid