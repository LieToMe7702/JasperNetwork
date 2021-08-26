#pragma once
#include "BufStream.h"
#include "TcpService.h"
#include "loop/EventLoop.h"
#include <arpa/inet.h>
#include <cstddef>
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
    void OnMessageSend();
    void RegisterMessageSendEvent(VoidEvent);
    void RegisterMessageReceiveEvent(MessageEvent);
    void Send(char *data, size_t len);

  private:
    int _port;
    uint32_t _addr;
    int _fd;
    std::shared_ptr<EventLoop> _runLoop;
    std::shared_ptr<EventHandler> _ioHandler;
    BufStream _inputStream;
    BufStream _outputStream;
    std::vector<VoidEvent> _messageSendEvent;
    std::vector<MessageEvent> _messageReceiveEvent;
};
} // namespace squid
