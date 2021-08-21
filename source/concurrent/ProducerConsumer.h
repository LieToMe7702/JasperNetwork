#pragma once
#include "BlockQueue.h"
#include <memory>
namespace squid
{
class ProducerConsumer
{
  public:
    void Test();

  private:
    BlockQueue<int> blockQueue;
};
} // namespace squid