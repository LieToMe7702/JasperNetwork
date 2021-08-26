#pragma once
#include <functional>
#include <memory>

namespace squid
{
class Connection;
class BufStream;
using MessageReceiveEvent = std::function<void(squid::Connection &connection, squid::BufStream &stream)>;
using ConnectionEvent = std::function<void(squid::Connection &connection)>;

} // namespace squid