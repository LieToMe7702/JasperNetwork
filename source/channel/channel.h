#pragma once
#include <memory>
#include <sys/epoll.h>
#include <vector>
namespace squid
{
enum class EventType
{
    Read = EPOLLIN | EPOLLPRI,
    Write = EPOLLOUT,
};
typedef std::vector<epoll_event> EventList;

class Channel
{
  public:
    void ModifyEvent(EventType type, bool enable);
    virtual void OnRead();
    virtual void OnWrite();
    virtual void OnClose();
    virtual void OnError();
    void SetFd(int fd);

  protected:
    int fd = -1;

  private:
    int event;
};
class EpollAcceptChannel : public Channel
{
  public:
    void OnRead() override;

  private:
    std::shared_ptr<EventList> eventList;
};
class ConnectionChannel : public Channel
{
};
} // namespace squid