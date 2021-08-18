#include "squid.h"

int main(int, char **)
{
    squid::TcpService service;
    service.Bind(8085);
    service.Run();
}
