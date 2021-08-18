#pragma once
#include "errorDefine.h"
namespace squid
{
class ErrorUtility
{
    public:
    static void LogError(squid::SocketError);
};
}