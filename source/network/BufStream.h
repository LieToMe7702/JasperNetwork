#pragma once
#include <cstddef>
#include <sys/types.h>
#include <vector>
namespace squid
{
class BufStream
{
  public:
    ssize_t ReadByFd(int fd);
    BufStream();

  private:
    std::vector<std::byte> _vec;
};
} // namespace squid
