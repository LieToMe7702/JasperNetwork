#include "squid.h"

int main(int, char **)
{
    fmt::print("hello format{0}\n", 1);
    squid::TcpService service;
    service.Bind(8085);
    service.Run();
}
