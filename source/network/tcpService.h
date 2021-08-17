#pragma once
namespace squid
{
class TcpService
{
  public:
    void Run();
    void Bind(int port);
    void SetSocketOption(int option,bool enable);
    ~TcpService();
  private:
    int listenFd = -1;
};
} // namespace squid