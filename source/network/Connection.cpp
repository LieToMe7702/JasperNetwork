#include "Connection.h"
#include "channel/EventType.h"
#include "network/TcpService.h"
#include "utility/utility.h"
#include <algorithm>
#include <functional>
#include <string_view>

using namespace squid;

Connection::Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop> loop)
    : _port(sockAddr.sin_port), _addr(sockAddr.sin_addr.s_addr), _fd(fd), _runLoop(loop), _ioHandler(new EventHandler),
      _connectionState(ConnectionState::Connected)
{
    _ioHandler->RegisterEvent(std::bind(&Connection::OnMessageReceiveFd, this, std::placeholders::_1), EventType::Read);
    _ioHandler->RegisterEvent(std::bind(&Connection::OnMessageSendFd, this, std::placeholders::_1), EventType::Write);
    _ioHandler->RegisterEvent(std::bind(&Connection::OnCloseFd, this, std::placeholders::_1), EventType::Close);
    _ioHandler->EnableReadEvent(true);
}
Connection::~Connection()
{
    CommonUtility::CloseFd(_fd);
}

void Connection::OnCloseFd(int fd)
{
    _runLoop->RunOnceInLoop([this, fd]() {
        _connectionState = ConnectionState::Disconnected;
        _runLoop->RegisterEventHandler(this->_ioHandler, fd, false);
        for (auto &it : _closeEvent)
        {
            it(*this);
        }
    });
}
void Connection::OnMessageReceiveFd(int fd)
{
    auto size = _inputStream.ReadFromFd(fd);
    if (size == 0)
    {
        OnCloseFd(fd);
    }
    else
    {
        OnMessageReceive(_inputStream);
    }
}
void Connection::OnMessageReceive(BufStream &stream)
{
    for (auto &it : _messageReceiveEvent)
    {
        it(*this, stream);
    }
}
void Connection::RegisterInLoop()
{
    _runLoop->RunOnceInLoop([this]() { _runLoop->RegisterEventHandler(_ioHandler, _fd); });
}
void Connection::OnMessageSendFd(int fd)
{
    auto n = _outputStream.WriteToFd(fd);
    if (n > 0)
    {
        if (_outputStream.Length() == 0)
        {
            _ioHandler->EnableWriteEvent(false);
            _runLoop->RegisterEventHandler(_ioHandler, fd);
            OnMessageSend();
            if (_connectionState == ConnectionState::Disconnection)
            {
                CloseWrite();
            }
        }
    }
}
void Connection::OnMessageSend()
{
    for (auto &it : _messageSendEvent)
    {
        it(*this);
    }
}
void Connection::RegisterCloseEvent(ConnectionEvent event)
{
    _closeEvent.emplace_back(event);
}
void Connection::RegisterMessageSendEvent(ConnectionEvent event)
{
    _messageSendEvent.emplace_back(event);
}
void Connection::RegisterMessageReceiveEvent(MessageReceiveEvent event)
{
    _messageReceiveEvent.emplace_back(event);
}

void Connection::Send(const char *data, size_t len)
{
    if (_runLoop->IsInLoopThread())
    {
        _outputStream.Write(data, len);
        _ioHandler->EnableWriteEvent(true);
        _runLoop->RegisterEventHandler(_ioHandler, _fd);
    }
    else
    {
        std::string str(data, len);
        _runLoop->RunOnceInLoop([str(std::move(str)), len, this]() mutable {
            _outputStream.Write(const_cast<char *>(str.data()), len);
            _ioHandler->EnableWriteEvent(true);
            _runLoop->RegisterEventHandler(_ioHandler, _fd);
        });
    }
}

void Connection::Close()
{
    _runLoop->RunOnceInLoop([this]() {
        if (_connectionState == ConnectionState::Connected)
        {
            _connectionState = ConnectionState::Disconnection;
        }
        CloseWrite();
    });
}
void Connection::CloseWrite()
{
    if (!_ioHandler->IsWrite())
    {
        ::shutdown(_fd, SHUT_WR);
    }
}
int Connection::Fd() const
{
    return _fd;
}
