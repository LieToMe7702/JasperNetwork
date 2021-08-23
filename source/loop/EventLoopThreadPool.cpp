#include "EventLoopThreadPool.h"
#include <algorithm>
#include <memory>
using namespace squid;
void EventLoopThreadPool::SetThreadNum(int numThread)
{
    _numThread = numThread;
}
void EventLoopThreadPool::Start()
{
    for (int i = 0; i < _numThread; i++)
    {
        std::unique_ptr<EventLoopThread> ptr(new EventLoopThread);
        _eventLoopThreads.emplace_back(std::move(ptr));
        _loops.emplace_back(std::move(ptr->StartLoop()));
    }
}

std::shared_ptr<EventLoop> EventLoopThreadPool::GetLoop()
{
    std::shared_ptr<EventLoop> loop = _baseLoop;
    if (_nextIndex < _loops.size())
    {
        loop = _loops[_nextIndex];
        _nextIndex++;
        _nextIndex %= _loops.size();
    }
    return loop;
}
EventLoopThreadPool::EventLoopThreadPool(std::shared_ptr<EventLoop> baseLoop) : _baseLoop(baseLoop)
{
}
