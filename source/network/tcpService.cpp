#include "tcpService.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/tcp.h>
#include <ostream>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
void squid::TcpService::Run()
{

    /*
    auto listenFd = socket(AF_INET, SOCK_STREAM, 0);
    auto enable = 1;
    auto res = 0;
    res = setsockopt(listenFd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));
    res = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    res = setsockopt(listenFd, SOL_SOCKET, TCP_NODELAY, &enable, sizeof(enable));
    struct sockaddr_in serverAddr, clientAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    auto size = sizeof(clientAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8080);
    auto bindRes = bind(listenFd, reinterpret_cast<const struct sockaddr *>(&serverAddr), sizeof(serverAddr));
    auto listenRes = listen(listenFd, 5);
    char buf[1000];
    while (true)
    {
        size = sizeof(clientAddr);
        auto connFd =
            accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr), reinterpret_cast<socklen_t *>(&size));
        std::cout << inet_ntop(AF_INET, &clientAddr.sin_addr, buf, sizeof(buf)) << std::endl;
        std::cout << clientAddr.sin_port;
        close(connFd);
    }*/
    // while(true){}
    if (listenFd == -1)
    {
        return;
    }
    struct sockaddr_in clientAddr;
    auto size = sizeof(clientAddr);
    char buf[1000];
    while (true)
    {
        size = sizeof(clientAddr);
        auto connFd =
            accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr), reinterpret_cast<socklen_t *>(&size));
        std::cout << connFd << std::endl;
        std::cout << inet_ntop(AF_INET, &clientAddr.sin_addr, buf, sizeof(buf)) << std::endl;
        std::cout << clientAddr.sin_port;
        close(connFd);
    }
}
void squid::TcpService::SetSocketOption(int option, bool enable)
{
    if (listenFd == -1)
    {
        return;
    }
    setsockopt(listenFd, SOL_SOCKET, option, &enable, sizeof(enable));
}
void squid::TcpService::Bind(int port)
{
    /*
        auto listenFd = socket(AF_INET, SOCK_STREAM, 0);
        auto enable = 1;
        auto res = 0;
        res = setsockopt(listenFd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(enable));
        res = setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
        res = setsockopt(listenFd, SOL_SOCKET, TCP_NODELAY, &enable, sizeof(enable));
        struct sockaddr_in serverAddr, clientAddr;
        bzero(&serverAddr, sizeof(serverAddr));
        auto size = sizeof(clientAddr);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(8080);
        auto bindRes = bind(listenFd, reinterpret_cast<const struct sockaddr *>(&serverAddr), sizeof(serverAddr));
        auto listenRes = listen(listenFd, 5);
        char buf[1000];
        while (true)
        {
            size = sizeof(clientAddr);
            auto connFd =
                accept(listenFd, reinterpret_cast<struct sockaddr *>(&clientAddr), reinterpret_cast<socklen_t
       *>(&size)); std::cout << inet_ntop(AF_INET, &clientAddr.sin_addr, buf, sizeof(buf)) << std::endl; std::cout <<
       clientAddr.sin_port; close(connFd);
        }*/

    struct addrinfo hints, *res;
    auto tmp = 0;
    std::shared_ptr<addrinfo> sharedRes;
    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (tmp = getaddrinfo(nullptr, std::to_string(port).c_str(), &hints, &res), sharedRes.reset(res); tmp != 0)
    {
        std::cout << "getaddrinfo error";
        return;
    }
    if (listenFd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); listenFd == -1)
    {
        std::cout << "socket error";
        return;
    }
    SetSocketOption(listenFd, SO_REUSEPORT);
    SetSocketOption(listenFd, SO_REUSEADDR);
    if (tmp = bind(listenFd, res->ai_addr, res->ai_addrlen); tmp != 0)
    {
        std::cout << "error";
        return;
    }
    if (tmp = listen(listenFd, 512); tmp != 0)
    {
        std::cout << "error";
        return;
    }
}

squid::TcpService::~TcpService()
{
    if (listenFd != -1)
    {
        close(listenFd);
    }
}