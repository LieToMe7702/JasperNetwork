#pragma once
#include "BufStream.h"
#include "Connection.h"
#include "TcpService.h"
#include "channel/EventHandler.h"
#include "loop//EventLoopThreadPool.h"
#include "loop/EventLoop.h"
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
namespace squid
{
using ConnectionEvent = std::function<void(Connection &stream)>;

class TcpServer /*: public TcpService*/
{
  public:
    void Run();
    void Bind(int port);
    void SetSocketOption(int option, bool enable);
    ~TcpServer();
    TcpServer(int threadCount = 0);

    void BuildNewConnection(int fd);
    void CloseConnection(int fd);

    void OnConnectionAccept(Connection &connection);
    void OnConnectionClose(Connection &connection);
    void RegisterMessageSendEvent(MessageEvent);
    void RegisterMessageReceiveEvent(MessageEvent);
    void RegisterConnectionAcceptEvent(ConnectionEvent);
    void RegisterConnectionCloseEvent(ConnectionEvent);

  private:
    int listenFd;
    int threadCount;
    int epollFd;
    std::shared_ptr<squid::EventHandler> connectionHandler;
    std::unordered_map<int, std::shared_ptr<Connection>> connectionMap;
    std::shared_ptr<squid::EventLoop> baseEventLoop;
    squid::EventLoopThreadPool _eventLoopThreadPool;
    std::vector<ConnectionEvent> _connectAcceptEvents;
    std::vector<ConnectionEvent> _connectCloseEvents;
    std::vector<MessageEvent> _messageSendEvent;
    std::vector<MessageEvent> _messageReceiveEvent;
};
} // namespace squid