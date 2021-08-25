#include "EventHandler.h"
#include "fmt/core.h"
#include "utility/squidUtility.h"
#include <utility>
using namespace squid;
void EventHandler::RegisterEvent(const EpollEvent &event, EventType type)
{
    auto &list = eventDict[type];
    list.emplace_back(event);
}

void EventHandler::RegisterEvent(EpollEvent &&event, EventType type)
{
    auto &list = eventDict[type];
    list.emplace_back(std::move(event));
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

const int EventHandler::readEventType = POLLIN | POLLPRI;
const int EventHandler::writeEventType = POLLOUT;

void EventHandler::EnableReadEvent(bool enable)
{
    if (enable)
        _enableEventType |= readEventType;
    else
    {
        _enableEventType &= ~readEventType;
    }
}
void EventHandler::EnableWriteEvent(bool enable)
{
    if (enable)
    {
        _enableEventType |= writeEventType;
    }
    else
    {
        _enableEventType &= ~writeEventType;
    }
}

int EventHandler::GetEnabledEventType() const
{
    return _enableEventType;
}
