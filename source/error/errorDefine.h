#pragma once
namespace squid
{
enum class SocketError
{
    GetAddrInfo = 1,
    CreateSocket,
    BindSocket,
    ListenScoket,
    AcceptSocket,
    EpollCreate,
    EpollAdd,
};
}