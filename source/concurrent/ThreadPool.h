#pragma once
#include <algorithm>
#include <cstddef>
#include <thread>
#include <vector>
namespace squid
{
class ThreadPool
{
  public:
    ThreadPool(size_t num);

  private:
    std::vector<std::unique_ptr<std::thread>> threads;
};
} // namespace squid