#pragma once
#include "Connection.h"
#include "Stream.h"
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
class TcpServer : public TcpService
{
  public:
    void Run();
    void Bind(int port);
    void SetSocketOption(int option, bool enable);
    ~TcpServer();
    TcpServer(int threadCount = 0);

    void BuildNewConnection(const int fd);
    virtual void OnMessageReceive(Stream &stream) override;
    virtual void OnMessageSend(Stream &stream) override;
    virtual void OnConnectionAccept(Connection &connection) override;
    virtual void OnConnectionClose(Connection &connection) override;
    virtual void RegisterMessageSendEvent(MessageEvent, bool enable) override;
    virtual void RegisterMessageReceiveEvent(MessageEvent, bool enable) override;
    virtual void RegisterConnectionAcceptEvent(ConnectionEvent, bool enable) override;
    virtual void RegisterConnectionCloseEvent(ConnectionEvent, bool enable) override;

  private:
    int listenFd;
    int threadCount;
    int epollFd;
    // squid::EventList eventList;
    // squid::EpollAcceptChannel channel;
    std::shared_ptr<squid::EventHandler> connectionHandler;
    std::vector<Connection> connectionVec;
    std::shared_ptr<squid::EventLoop> baseEventLoop;
    squid::EventLoopThreadPool _eventLoopThreadPool;
};
} // namespace squid