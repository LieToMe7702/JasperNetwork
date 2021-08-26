#pragma once
#include <functional>
#include <memory>
#include <sys/epoll.h>
#include <sys/poll.h>

namespace squid
{
using EpollEvent = std::function<void(int fd)>;
enum class EventType
{
    None,
    Read,
    Write,
    Close,
    Error,
};
} // namespace squid
