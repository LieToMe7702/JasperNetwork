#pragma once
#include "EventTypeUtility.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
namespace squid
{
using EventDictType = std::unordered_map<EventType, std::vector<EpollEvent>>;
class EventHandler /*: public std::enable_shared_from_this<EventHandler>*/
{
  public:
    void RegisterEvent(const EpollEvent &event, EventType type);
    void RegisterEvent(EpollEvent &&event, EventType type);
    void Handle(EventType eventType, int socketFd);
    const EventDictType &GetEventType();

  private:
    EventDictType eventDict;
};
} // namespace squid