#include "ProducerConsumer.h"
#include "utility/squidUtility.h"
#include <thread>
using namespace squid;
void ProducerConsumer::Test()
{
    auto thread1 = std::thread([&]() {
        for (int i = 0; i < 100; i++)
        {
            blockQueue.Put(i);
            fmt::print("put data:{0}\n", i);
        }
    });
    auto thread2 = std::thread([&]() {
        for (int i = 0; i < 100; i++)
        {
            auto val = blockQueue.Take();
            fmt::print("take data:{0}\n", val);
        }
    });
    thread1.detach();
    thread2.detach();
}