#pragma once
#include "EventTypeDefine.h"
namespace squid
{
class EventHandler
{
  public:
    void RegisterEvent(EpollEvent event,EventType type,bool enable=true);
};
}