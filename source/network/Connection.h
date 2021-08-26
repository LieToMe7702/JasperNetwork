#pragma once
#include "BufStream.h"
#include "TcpService.h"
#include "loop/EventLoop.h"
#include <arpa/inet.h>
#include <cstddef>
#include <memory>

namespace squid
{
enum class ConnectionState
{
    Connected,
    Disconnection,
    Disconnected,
};
class Connection /*: public TcpService*/
{
  public:
    Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop>);
    ~Connection();
    void OnMessageReceiveFd(int fd);
    void OnMessageReceive(BufStream &stream);
    void OnMessageSendFd(int fd);
    void OnMessageSend();
    void OnCloseFd(int fd);

    void RegisterMessageSendEvent(ConnectionEvent);
    void RegisterMessageReceiveEvent(MessageReceiveEvent);
    void RegisterCloseEvent(ConnectionEvent);
    void Send(const char *data, size_t len);
    void Close();
    int Fd() const;
    void RegisterInLoop();

  private:
    void CloseWrite();
    int _port;
    uint32_t _addr;
    int _fd;
    std::shared_ptr<EventLoop> _runLoop;
    std::shared_ptr<EventHandler> _ioHandler;
    BufStream _inputStream;
    BufStream _outputStream;
    std::vector<ConnectionEvent> _messageSendEvent;
    std::vector<MessageReceiveEvent> _messageReceiveEvent;
    ConnectionState _connectionState;
    std::vector<ConnectionEvent> _closeEvent;
};
} // namespace squid
