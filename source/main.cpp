#include "fmt/core.h"
#include "squid.h"
#include <thread>

int main(int, char **)
{
    /*
    squid::ProducerConsumer producerConsumer;
    producerConsumer.Test();*/
    squid::TcpServer service;
    service.Bind(8080);
    service.Run();
}
