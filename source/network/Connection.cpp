#include "Connection.h"
#include "channel/EventType.h"
#include <functional>
using namespace squid;

Connection::Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop> loop)
    : _port(sockAddr.sin_port), _addr(sockAddr.sin_addr.s_addr), _fd(fd), _runLoop(loop), _ioHandler(new EventHandler)
{
    _ioHandler->RegisterEvent(std::bind(&Connection::OnMessageReceiveFd, this, std::placeholders::_1), EventType::Read);
    _ioHandler->RegisterEvent(std::bind(&Connection::OnMessageSendFd, this, std::placeholders::_1), EventType::Write);
    _ioHandler->EnableReadEvent(true);
}
void Connection::OnMessageReceiveFd(int fd)
{
}
void Connection::OnMessageReceive(BufStream &stream)
{
}
void Connection::OnMessageSendFd(int fd)
{
}
void Connection::OnMessageSend(BufStream &stream)
{
}
void Connection::RegisterMessageSendEvent(MessageEvent)
{
}
void Connection::RegisterMessageReceiveEvent(MessageEvent)
{
}
