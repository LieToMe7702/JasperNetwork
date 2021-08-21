#include "EventTypeUtility.h"
using namespace squid;
uint32_t EventTypeUtility::EventTypeToEpollEvents(EventType eventType)
{
    return eventTypeToEpollTypeDict[eventType];
}
std::unordered_map<EventType, uint32_t> EventTypeUtility::eventTypeToEpollTypeDict{
    {EventType::Read, POLLIN | POLLPRI}, {EventType::Write, POLLOUT}, {EventType::Error, POLLERR}};