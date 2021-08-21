#pragma once
#include "network/Socket.h"
#include <functional>
#include <sys/epoll.h>
#include <sys/poll.h>

namespace squid
{
using EpollEvent = std::function<void(int fd)>;
enum class EventType
{
    None,
    Read,
    Write,
    Close,
    Error,
};
class EventTypeUtility
{
  public:
    static uint32_t EventTypeToEpollEvents(EventType eventType);

  private:
    static std::unordered_map<EventType, uint32_t> eventTypeToEpollTypeDict;
};
/*
static inline std::unordered_map<EventType, uint32_t> eventTypeToEpollTypeDict{
    {EventType::Read, EPOLLIN | EPOLLPRI}, {EventType::Write, EPOLLOUT}, {EventType::Error, EPOLLERR}};

static inline uint32_t EventTypeToEpollEvents(EventType eventType)
{
    return eventTypeToEpollTypeDict[eventType];
}*/

/*
enum class EventType
{
    Read = EPOLLIN | EPOLLPRI,
    Write = EPOLLOUT,
    Error = EPOLLERR,
};*/
} // namespace squid
