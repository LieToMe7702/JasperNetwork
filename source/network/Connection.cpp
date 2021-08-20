#include "Connection.h"
using namespace squid;

Connection::Connection(sockaddr_in &sockAddr) : port(sockAddr.sin_port), addr(sockAddr.sin_addr.s_addr)
{
}
