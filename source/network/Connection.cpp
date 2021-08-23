#include "Connection.h"
using namespace squid;

Connection::Connection(sockaddr_in &sockAddr) : _port(sockAddr.sin_port), _addr(sockAddr.sin_addr.s_addr)
{
}
