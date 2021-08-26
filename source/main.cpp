#include "fmt/core.h"
#include "squid.h"
#include <thread>

int main(int, char **)
{
    /*
    squid::ProducerConsumer producerConsumer;
    producerConsumer.Test();*/
    /*
    squid::ThreadPool threadPool(6);
    for (int i = 0; i < 20; i++)
    {
        threadPool.Put([=]() { fmt::print("i={0}\n", i); });
    }*/
    squid::HttpServer service(8);
    service.Start(8080);
}
