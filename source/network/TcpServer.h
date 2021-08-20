#pragma once
#include "Connection.h"
#include "Socket.h"
#include "Stream.h"
#include "squid.h"
#include <memory>
#include <unordered_map>
#include <vector>
namespace squid
{
using MessageEvent = std::function<void(Stream &stream)>;
using ConnectionEvent = std::function<void(Connection &stream)>;
class TcpServer
{
  public:
    void Run();
    void Bind(int port);
    void SetSocketOption(int option, bool enable);
    ~TcpServer();
    TcpServer(int threadCount = 0);
    void OnMessageReceive(Stream &stream);
    void OnMessageSend(Stream &stream);
    void OnConnectionAccept(Connection &connection);
    void OnConnectionClose(Connection &connection);
    void RegisterMessageSendEvent(MessageEvent, bool enable);
    void RegisterMessageReceiveEvent(MessageEvent, bool enable);
    void RegisterConnectionAcceptEvent(ConnectionEvent, bool enable);
    void RegisterConnectionCloseEvent(ConnectionEvent, bool enable);
    void BuildNewConnection(const Socket &socket);

  private:
    int listenFd = -1;
    int threadCount;
    int epollFd = -1;
    squid::EventList eventList;
    squid::EpollAcceptChannel channel;
    squid::EventHandler connectionHandler;
    std::vector<Connection> connectionVec;
};
} // namespace squid