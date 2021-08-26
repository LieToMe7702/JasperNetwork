#include "Connection.h"
#include "channel/EventType.h"
#include <algorithm>
#include <functional>
#include <string_view>
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
    _inputStream.ReadFromFd(fd);
    OnMessageReceive(_inputStream);
}
void Connection::OnMessageReceive(BufStream &stream)
{
    for (auto &it : _messageReceiveEvent)
    {
        it(stream);
    }
}
void Connection::OnMessageSendFd(int fd)
{
    auto n = _outputStream.WriteToFd(fd);
    if (n > 0)
    {
        if (_outputStream.Length() == 0)
        {
            _ioHandler->EnableWriteEvent(false);
            OnMessageSend();
        }
    }
}
void Connection::OnMessageSend()
{
    for (auto &it : _messageSendEvent)
    {
        it();
    }
}

void Connection::RegisterMessageSendEvent(VoidEvent event)
{
    _messageSendEvent.emplace_back(event);
}
void Connection::RegisterMessageReceiveEvent(MessageEvent event)
{
    _messageReceiveEvent.emplace_back(event);
}

void Connection::Send(char *data, size_t len)
{
    if (_runLoop->IsInLoopThread())
    {
        _outputStream.Write(data, len);
        _ioHandler->EnableWriteEvent(true);
        _runLoop->UpdateEventHandlerByFd(_fd);
    }
    else
    {
        std::string str(data, len);
        _runLoop->RunOnceInLoop([str(std::move(str)), len, this]() mutable {
            _outputStream.Write(const_cast<char *>(str.data()), len);
            _ioHandler->EnableWriteEvent(true);
            _runLoop->UpdateEventHandlerByFd(_fd);
        });
    }
}
