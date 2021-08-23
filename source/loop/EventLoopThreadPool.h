#pragma once
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <algorithm>
#include <memory>
namespace squid
{
class EventLoopThreadPool
{
  public:
    void SetThreadNum(int numThread);
    void Start();
    std::shared_ptr<squid::EventLoop> GetLoop();
    EventLoopThreadPool(std::shared_ptr<squid::EventLoop> baseLoop);

  private:
    int _numThread;
    int _nextIndex;
    std::vector<std::unique_ptr<squid::EventLoopThread>> _eventLoopThreads;
    std::vector<std::shared_ptr<squid::EventLoop>> _loops;
    std::shared_ptr<squid::EventLoop> _baseLoop;
};
} // namespace squid
