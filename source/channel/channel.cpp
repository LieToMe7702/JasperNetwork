#include "channel.h"
using namespace squid;
void Channel::OnRead(){};
void Channel::OnClose(){};
void Channel::OnError(){};
void Channel::OnWrite(){};
void Channel::ModifyEvent(EventType type, bool enable)
{
    if (enable)
    {
        event |= static_cast<int>(type);
    }
    else
    {
        event &= ~static_cast<int>(type);
    }
}
void Channel::SetFd(int fd)
{
    this->fd = fd;
}
