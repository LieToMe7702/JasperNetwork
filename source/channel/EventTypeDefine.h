#pragma once
#include "network/Socket.h"
#include <functional>
#include <sys/epoll.h>

namespace squid
{
using EpollEvent = std::function<void(const Socket &socket)>;
enum class EventType
{
    Read = EPOLLIN | EPOLLPRI,
    Write = EPOLLOUT,
    Error = EPOLLERR,
};
} // namespace squid
