#pragma once
#include <netinet/in.h>
namespace squid
{
class Socket
{
  private:
    in_addr addr;
    int port;
};
} // namespace squid
