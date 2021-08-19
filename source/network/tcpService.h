#pragma once
#include "squid.h"
#include <memory>
namespace squid
{
class TcpService
{
  public:
    void Run();
    void Bind(int port);
    void SetSocketOption(int option,bool enable);
    ~TcpService();
    TcpService(int threadCount=0);
  private:
    int listenFd = -1;
    int threadCount;
    int epollFd = -1;
    squid::EventList eventList;
    squid::EpollAcceptChannel channel;
};
} // namespace squid