#pragma once
#include "BufStream.h"
#include <functional>
#include <memory>

namespace squid
{
using MessageEvent = std::function<void(BufStream &stream)>;
using VoidEvent = std::function<void()>;

} // namespace squid