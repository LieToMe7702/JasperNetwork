#pragma once
#include <sys/epoll.h>
namespace squid {
class channel
{
    epoll_event event;
};
}