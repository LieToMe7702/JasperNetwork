#include "errorUtility.h"
#include "fmt/printf.h"
using namespace squid;
void ErrorUtility::LogError(SocketError error)
{
    fmt::print("SocketError:{0}\n",error);
}