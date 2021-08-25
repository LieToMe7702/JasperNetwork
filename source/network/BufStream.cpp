#include "BufStream.h"
#include <algorithm>
#include <bits/types/struct_iovec.h>
#include <sys/uio.h>
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
    auto readableSize = ReadableSize();
    if (Pos() > 2 * readableSize)
    {
        std::copy(_vec.begin() + Pos(), _vec.begin() + Length(), _vec.begin());
        SetPos(0);
        SetLength(readableSize);
    }
}

void BufStream::AutoResize(size_t len)
{
    if (Length() + len >= Capacity())
    {
        _vec.resize(3 * Length() / 2);
    }
}

void BufStream::EnsureCapacity(size_t len)
{
    FillGap();
    AutoResize(len);
}

void BufStream::Write(char *bytes, size_t len)
{
    auto res = reinterpret_cast<std::byte *>(bytes);
    Write(res, len);
}

void BufStream::Write(std::byte *bytes, size_t len)
{
    EnsureCapacity(len);
    auto end = Length();
    SetLength(end + len);
    std::copy(bytes, bytes + len, _vec.begin() + end);
}

int BufStream::Read(char *bytes, size_t len)
{
    auto res = reinterpret_cast<std::byte *>(bytes);
    return Read(res, len);
}

int BufStream::Read(std::byte *bytes, size_t len)
{
    auto targetEnd = Pos() + len;
    targetEnd = std::min(targetEnd, Length());
    std::copy(_vec.begin() + Pos(), _vec.begin() + targetEnd, bytes);
    auto res = targetEnd - Pos();
    SetPos(targetEnd);
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
    return _writeIndex;
}

void BufStream::SetLength(size_t pos)
{
    _writeIndex = pos;
}

bool BufStream::HaveReadableData()
{
    return _writeIndex > _readIndex;
}

size_t BufStream::ReadableSize()
{
    auto res = HaveReadableData() ? _writeIndex - _readIndex : 0;
    return res;
}

void BufStream::Clear()
{
    _readIndex = 0;
}

thread_local BufStream BufStream::commonBufStream(65536);
std::byte *BufStream::GetByteArray()
{
    return &*_vec.begin();
}

ssize_t BufStream::ReadByFd(int fd)
{
    struct iovec vec[2];
    auto writeBytes = Capacity() - Length();
    vec[0].iov_base = GetByteArray() + Length();
    vec[0].iov_len = writeBytes;
    auto commonBufArray = BufStream::commonBufStream.GetByteArray();
    vec[1].iov_base = commonBufArray;
    vec[1].iov_len = sizeof(commonBufArray);
    auto count = writeBytes < sizeof(commonBufArray) ? 2 : 1;
    auto readCount = readv(fd, vec, count);
    if (readCount < 0)
    {
        return -1;
    }
    else if (readCount < writeBytes)
    {
        SetPos(Length() + readCount);
    }
    else
    {
        SetPos(Capacity());
        Write(commonBufArray, readCount - writeBytes);
    }
    return readCount;
}
