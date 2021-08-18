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
    std::shared_ptr<EventList> eventList;
    EpollAcceptChannel channel;
};
} // namespace squid