#include "channel.h"
#include "squid.h"
#include <strings.h>
#include <sys/epoll.h>
using namespace squid;

void EpollAcceptChannel::OnRead()
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    eventList->push_back(event);
    if (auto res = epoll_ctl(fd, EPOLL_CTL_ADD, fd, &event); res == -1)
    {
        ErrorUtility::LogError(SocketError::EpollAdd);
        return;
    }
}