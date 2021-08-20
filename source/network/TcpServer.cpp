#include "TcpServer.h"
#include "channel/EventTypeDefine.h"
#include "error/errorUtility.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <functional>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/tcp.h>
#include <ostream>
#include <squid.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
using namespace squid;
void squid::TcpServer::Run()
{
    if (listenFd == -1)
    {
        return;
    }
    baseEventLoop.Loop();
    /*
     if (epollFd = epoll_create1(EPOLL_CLOEXEC); epollFd == -1)
     {
         ErrorUtility::LogError(SocketError::EpollCreate);
         return;
     }
     struct epoll_event event;
     bzero(&event, sizeof(event));

     event.events = static_cast<uint32_t>(EventType::Read);
     if (auto res = epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd, &event); res == -1)
     {
         ErrorUtility::LogError(SocketError::EpollAdd);
         return;
     }

     struct sockaddr_in clientAddr;
     auto size = sizeof(clientAddr);
     char buf[1000];
     std::vector<epoll_event> vec(20);
     while (true)
     {
         if (epoll_wait(epollFd, &*vec.begin(), 1024, 10) > 0)
         {
             size = sizeof(clientAddr);
             auto connFd = accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr),
                                  reinterpret_cast<socklen_t *>(&size));
             std::cout << connFd << std::endl;
             close(connFd);
         }
     }*/
}
void squid::TcpServer::SetSocketOption(int option, bool enable)
{
    if (listenFd == -1)
    {
        return;
    }
    setsockopt(listenFd, SOL_SOCKET, option, &enable, sizeof(enable));
}
void squid::TcpServer::Bind(int port)
{
    struct addrinfo hints, *res;
    auto tmp = 0;
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    // hints.ai_socktype = SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC;
    hints.ai_socktype = SOCK_STREAM;
    if (tmp = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &res); tmp != 0)
    {
        ErrorUtility::LogError(SocketError::GetAddrInfo);
        return;
    }
    std::shared_ptr<addrinfo> sharedRes{res};
    res->ai_socktype |= (SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (listenFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); listenFd == -1)
    {
        ErrorUtility::LogError(SocketError::CreateSocket);
        return;
    }
    SetSocketOption(listenFd, SO_REUSEPORT);
    SetSocketOption(listenFd, SO_REUSEADDR);
    if (tmp = bind(listenFd, res->ai_addr, res->ai_addrlen); tmp != 0)
    {
        ErrorUtility::LogError(SocketError::BindSocket);
        return;
    }
    if (tmp = listen(listenFd, 512); tmp != 0)
    {
        ErrorUtility::LogError(SocketError::ListenScoket);
        return;
    }

    connectionHandler->RegisterEvent(std::bind(&TcpServer::BuildNewConnection, this, std::placeholders::_1),
                                     EventType::Read);
    baseEventLoop.RegisterEventHandler(connectionHandler, listenFd);
}

TcpServer::~TcpServer()
{
    if (listenFd != -1)
    {
        close(listenFd);
    }
    if (epollFd != -1)
    {
        close(epollFd);
    }
}
void TcpServer::BuildNewConnection(int fd)
{
    struct sockaddr_in clientAddr;
    auto size = sizeof(clientAddr);
    auto connFd =
        accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr), reinterpret_cast<socklen_t *>(&size));
    if (connFd == -1)
    {
        ErrorUtility::LogError(SocketError::AcceptSocket);
        return;
    }
    connectionVec.emplace_back(clientAddr);
}
TcpServer::TcpServer(int threadCount) : threadCount(threadCount), connectionHandler(new EventHandler)
{
}
void TcpServer::OnMessageReceive(Stream &stream)
{
}
void TcpServer::OnConnectionAccept(Connection &connection)
{
}
void TcpServer::OnMessageSend(Stream &stream)
{
}
void TcpServer::OnConnectionClose(Connection &connection)
{
}
void TcpServer::RegisterMessageSendEvent(MessageEvent, bool enable)
{
}

void TcpServer::RegisterMessageReceiveEvent(MessageEvent, bool enable)
{
}
void TcpServer::RegisterConnectionAcceptEvent(ConnectionEvent, bool enable)
{
}
void TcpServer::RegisterConnectionCloseEvent(ConnectionEvent, bool enable)
{
}
