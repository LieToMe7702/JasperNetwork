#pragma once
#include <cstddef>
#include <sys/types.h>
#include <vector>
namespace squid
{
class BufStream
{
  public:
    ssize_t ReadFromFd(int fd);
    ssize_t WriteToFd(int fd);
    BufStream(size_t size = 1024);

    size_t Capacity() const;
    int Read(char *bytes, size_t len);
    int Read(std::byte *bytes, size_t len);
    void Write(const char *bytes, size_t len);
    void Write(const std::byte *bytes, size_t len);
    void SetPos(size_t pos);
    size_t Pos() const;
    size_t Length() const;
    bool HaveReadableData();
    void Clear();
    std::byte *GetByteArray();

  private:
    std::vector<std::byte> _vec;
    size_t _readIndex;
    size_t _writeIndex;
    void EnsureCapacity(size_t len);
    void SetLength(size_t len);
    void FillGap();
    void AutoResize(size_t len);
    thread_local static char commonBufStream[];
};
} // namespace squid
