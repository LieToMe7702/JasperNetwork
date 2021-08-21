#include "EventHandler.h"
#include "fmt/core.h"
#include "utility/squidUtility.h"
#include <utility>
using namespace squid;
void EventHandler::RegisterEvent(EpollEvent event, EventType type)
{
    auto &list = eventDict[type];
    list.push_back(std::forward<EpollEvent>(event));
}

void EventHandler::Handle(EventType eventType, int socketFd)
{
    auto it = eventDict.find(eventType);
    if (it == eventDict.end())
        return;
    for (auto &func : it->second)
    {
        func(socketFd);
    }
}
const EventDictType &EventHandler::GetEventType()
{
    return eventDict;
}
