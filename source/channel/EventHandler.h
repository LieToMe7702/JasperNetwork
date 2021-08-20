#pragma once
#include "EventTypeDefine.h"
#include <memory>
namespace squid
{
class EventHandler /*: public std::enable_shared_from_this<EventHandler>*/
{
  public:
    void RegisterEvent(EpollEvent event, EventType type, bool enable = true);
    void Handle(int eventType, int socketFd);
};
} // namespace squid