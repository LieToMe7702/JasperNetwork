#include "Connection.h"
using namespace squid;

Connection::Connection(sockaddr_in &sockAddr, int fd, std::shared_ptr<EventLoop> loop)
    : _port(sockAddr.sin_port), _addr(sockAddr.sin_addr.s_addr), _fd(fd), _runLoop(loop), _ioHandler(new EventHandler)
{
}
