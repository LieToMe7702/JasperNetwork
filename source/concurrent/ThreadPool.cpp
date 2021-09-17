#include "ThreadPool.h"
#include <algorithm>
#include <memory>
using namespace squid;
ThreadPool::ThreadPool(int size)
{
    threads.reserve(size);
    for (int i = 0; i < size; i++)
    {
        std::unique_ptr<std::thread> ptr(new std::thread(std::bind(&ThreadPool::RunInThread, this)));
        threads.emplace_back(std::move(ptr));
    }
}
void ThreadPool::Put(const ThreadFuncType &func)
{
    blockQueue.Put(func);
}
void ThreadPool::Put(ThreadFuncType &&func)
{
    blockQueue.Put(std::move(func));
}

void ThreadPool::RunInThread()
{
    while (true)
    {
        blockQueue.Take()();
    }
}