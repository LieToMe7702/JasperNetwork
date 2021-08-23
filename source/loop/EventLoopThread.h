#pragma once
#include "EventLoop.h"
#include <condition_variable>
#include <memory>
#include <thread>
namespace squid
{
class EventLoopThread
{
  public:
    std::shared_ptr<EventLoop> StartLoop();
    EventLoopThread();

  private:
    void ThreadFunc();
    std::shared_ptr<EventLoop> _loop;
    std::thread _thread;
    std::mutex _lock;
    std::condition_variable _cond;
};
} // namespace squid