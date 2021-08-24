#pragma once
#include "BufStream.h"
#include "TcpService.h"
#include "loop/EventLoop.h"
#include <arpa/inet.h>
#include <memory>

namespace squid
{
class Connection /*: public TcpService*/
{
  public:
    Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop>);
    void OnMessageReceiveFd(int fd);
    void OnMessageReceive(BufStream &stream);
    void OnMessageSendFd(int fd);
    void OnMessageSend(BufStream &stream);
    void RegisterMessageSendEvent(MessageEvent);
    void RegisterMessageReceiveEvent(MessageEvent);

  private:
    int _port;
    uint32_t _addr;
    int _fd;
    std::shared_ptr<EventLoop> _runLoop;
    std::shared_ptr<EventHandler> _ioHandler;
    BufStream _bufStream;
};
} // namespace squid
