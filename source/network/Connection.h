#pragma once
#include <arpa/inet.h>
namespace squid
{
class Connection
{
  public:
    Connection(sockaddr_in &sockAddr);

  private:
    int port;
    uint32_t addr;
};
} // namespace squid
