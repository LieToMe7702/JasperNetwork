#pragma once
#include "BufStream.h"
#include <functional>
#include <memory>

namespace squid
{
using MessageEvent = std::function<void(BufStream &stream)>;
class TcpService
{
  public:
    virtual void OnMessageReceive(BufStream &stream) = 0;
    virtual void OnMessageSend(BufStream &stream) = 0;
    virtual void RegisterMessageSendEvent(MessageEvent) = 0;
    virtual void RegisterMessageReceiveEvent(MessageEvent) = 0;
};
} // namespace squid