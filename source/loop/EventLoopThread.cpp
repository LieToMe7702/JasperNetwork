#include "EventLoopThread.h"
#include <memory>
#include <thread>
using namespace squid;
std::shared_ptr<EventLoop> EventLoopThread::StartLoop()
{
    std::shared_ptr<EventLoop> loop;
    {
        std::unique_lock guard(_lock);
        while (_loop == nullptr)
        {
            _cond.wait(guard);
        }
        loop = _loop;
    }
    return loop;
}
void EventLoopThread::ThreadFunc()
{
    {
        std::unique_lock guard(_lock);
        _loop.reset(new EventLoop);
        _cond.notify_one();
    }

    _loop->Loop();
    std::unique_lock guard(_lock);
    _loop.reset();
}
EventLoopThread::EventLoopThread() : _thread(std::bind(&EventLoopThread::ThreadFunc, this))
{
}
