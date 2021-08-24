#pragma once
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
namespace squid
{
template <typename T> class BlockQueue
{
  public:
    void Put(const T &x)
    {
        std::unique_lock guard(lock);
        internalQueue.emplace(x);
        notEmpty.notify_one();
    }
    void Put(const T &&x)
    {
        std::unique_lock guard(lock);
        internalQueue.emplace(std::forward(x));
        notEmpty.notify_one();
    }
    T Take()
    {
        std::unique_lock guard(lock);
        while (internalQueue.empty())
        {
            notEmpty.wait(guard);
        }
        T front(std::move(internalQueue.front()));
        internalQueue.pop();
        return front;
    }
    size_t size() const
    {
        std::lock_guard guard(lock);
        return internalQueue.size();
    }

  private:
    std::mutex lock;
    std::condition_variable notEmpty;
    std::queue<T> internalQueue;
};

} // namespace squid