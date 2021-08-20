#include "squid.h"

int main(int, char **)
{
    squid::TcpServer service;
    service.Bind(8085);
    service.Run();
}
