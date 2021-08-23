#pragma once
#include "Stream.h"
#include "loop/EventLoop.h"
#include <arpa/inet.h>
#include <memory>

namespace squid
{
class Connection
{
  public:
    Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop>);

  private:
    int _port;
    uint32_t _addr;
    int _fd;
    std::shared_ptr<EventLoop> _runLoop;
    std::shared_ptr<EventHandler> _ioHandler;
    Stream _bufSteam;
};
} // namespace squid
