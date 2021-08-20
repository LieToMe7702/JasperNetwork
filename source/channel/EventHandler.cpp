#include "EventHandler.h"
#include "fmt/core.h"
#include "utility/squidUtility.h"
using namespace squid;
void EventHandler::RegisterEvent(EpollEvent event, EventType type, bool enable)
{
}

void EventHandler::Handle(int eventType, int socketFd)
{
    fmt::print("Handle,eventType={0},socketFd={1}\n", eventType, socketFd);
    Utility::CloseFd(socketFd);
}
