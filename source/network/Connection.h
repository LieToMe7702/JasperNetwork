#pragma once
#include "loop/EventLoop.h"
#include <arpa/inet.h>
#include <memory>

namespace squid
{
class Connection
{
  public:
    Connection(sockaddr_in &sockAddr);

  private:
    int _port;
    uint32_t _addr;
    int _fd;
    std::weak_ptr<EventLoop> _runLoop;
};
} // namespace squid
