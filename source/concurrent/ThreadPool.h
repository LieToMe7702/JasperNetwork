#pragma once
#include "BlockQueue.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <thread>
#include <vector>
namespace squid
{
using ThreadFuncType = std::function<void()>;
class ThreadPool
{
  public:
    ThreadPool(int num);
    void Put(const ThreadFuncType &func);
    void Put(ThreadFuncType &&func);

  private:
    void RunInThread();
    std::vector<std::unique_ptr<std::thread>> threads;
    BlockQueue<ThreadFuncType> blockQueue;
};
} // namespace squid