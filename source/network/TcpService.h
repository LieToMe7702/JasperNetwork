#pragma once
#include "Connection.h"
#include "Stream.h"
#include <functional>
#include <memory>

namespace squid
{
using MessageEvent = std::function<void(Stream &stream)>;
using ConnectionEvent = std::function<void(Connection &stream)>;
class TcpService
{
  public:
    virtual void OnMessageReceive(Stream &stream) = 0;
    virtual void OnMessageSend(Stream &stream) = 0;
    virtual void OnConnectionAccept(Connection &connection) = 0;
    virtual void OnConnectionClose(Connection &connection) = 0;
    virtual void RegisterMessageSendEvent(MessageEvent, bool enable) = 0;
    virtual void RegisterMessageReceiveEvent(MessageEvent, bool enable) = 0;
    virtual void RegisterConnectionAcceptEvent(ConnectionEvent, bool enable) = 0;
    virtual void RegisterConnectionCloseEvent(ConnectionEvent, bool enable) = 0;
};
} // namespace squid