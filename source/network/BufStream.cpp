#include "BufStream.h"
#include <algorithm>
#include <bits/types/struct_iovec.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace squid;

BufStream::BufStream(size_t size) : _vec(size)
{
}

size_t BufStream::Capacity() const
{
    return _vec.size();
}

void BufStream::FillGap()
{
    auto readableSize = Length();
    if (Pos() > 2 * readableSize)
    {
        std::copy(_vec.begin() + _readIndex, _vec.begin() + _writeIndex, _vec.begin());
        _writeIndex -= _readIndex;
        _readIndex = 0;
    }
}

void BufStream::AutoResize(size_t len)
{
    if (_writeIndex + len >= Capacity())
    {
        _vec.resize(3 * Capacity() / 2);
    }
}

void BufStream::EnsureCapacity(size_t len)
{
    FillGap();
    AutoResize(len);
}

void BufStream::Write(const char *bytes, size_t len)
{
    auto res = reinterpret_cast<const std::byte *>(bytes);
    Write(res, len);
}

void BufStream::Write(const std::byte *bytes, size_t len)
{
    EnsureCapacity(len);
    std::copy(bytes, bytes + len, _vec.begin() + _writeIndex);
    _writeIndex += len;
}

int BufStream::Read(char *bytes, size_t len)
{
    auto res = reinterpret_cast<std::byte *>(bytes);
    return Read(res, len);
}

int BufStream::Read(std::byte *bytes, size_t len)
{
    auto targetEnd = _readIndex + len;
    targetEnd = std::min(targetEnd, _writeIndex);
    std::copy(_vec.begin() + _readIndex, _vec.begin() + targetEnd, bytes);
    auto res = targetEnd - _readIndex;
    _readIndex = targetEnd;
    return res;
}

size_t BufStream::Pos() const
{
    return _readIndex;
}

void BufStream::SetPos(size_t pos)
{
    auto res = std::min(_writeIndex, pos);
    _readIndex = res;
}

size_t BufStream::Length() const
{
    return _writeIndex - _readIndex;
}

void BufStream::SetLength(size_t len)
{
    _writeIndex = _readIndex + len;
}

bool BufStream::HaveReadableData()
{
    return _writeIndex > _readIndex;
}

void BufStream::Clear()
{
    _readIndex = 0;
    _writeIndex = 0;
}

thread_local char BufStream::commonBufStream[65536];
std::byte *BufStream::GetByteArray()
{
    return &*(_vec.begin() + _readIndex);
}

ssize_t BufStream::ReadFromFd(int fd)
{
    struct iovec vec[2];
    char extrabuf[65536];
    auto writeBytes = Capacity() - _writeIndex;
    vec[0].iov_base = GetByteArray() + Length();
    vec[0].iov_len = writeBytes;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof(extrabuf);
    auto count = writeBytes < vec[1].iov_len ? 2 : 1;
    auto readCount = readv(fd, vec, count);
    if (readCount < 0)
    {
        return -1;
    }
    else if (readCount < writeBytes)
    {
        _writeIndex += readCount;
    }
    else
    {
        _writeIndex = Capacity();
        Write(extrabuf, readCount - writeBytes);
    }
    return readCount;
}
ssize_t BufStream::WriteToFd(int fd)
{
    std::string test{"<html>\
<head><title>这是我的第一个html</title></head>\
<body>\
Hello World\
</body>\
</html>\
"};
    // auto n = ::write(fd, GetByteArray(), Length());
    auto n = ::write(fd, test.c_str(), test.size());
    if (n > 0)
    {
        _readIndex += n;
    }
    return n;
}
