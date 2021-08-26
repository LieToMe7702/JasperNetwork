#pragma once
#include "TcpServer.h"
namespace squid
{
class HttpServer
{
  public:
    HttpServer(int threadCount = 0);
    void Start(int port);
    void OnMessageReceive(Connection &connection, BufStream &stream);

  private:
    TcpServer _server;
};
} // namespace squid