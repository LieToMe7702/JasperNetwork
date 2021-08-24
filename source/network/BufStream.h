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
    BufStream(size_t size = 1024);

    size_t Capacity() const;
    int Read(char *bytes, size_t len);
    int Read(std::byte *bytes, size_t len);
    void Write(char *bytes, size_t len);
    void Write(std::byte *bytes, size_t len);
    void SetPos(size_t pos);
    size_t Pos() const;
    void SetLength(size_t len);
    size_t Length();
    bool HaveReadableData();

  private:
    std::vector<std::byte> _vec;
    size_t _pos;
    int _length;
    void EnsureCapacity();
};
} // namespace squid
