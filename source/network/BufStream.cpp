#include "BufStream.h"
#include <algorithm>
using namespace squid;
BufStream::BufStream(size_t size) : _vec(size)
{
}
size_t BufStream::Capacity() const
{
    return _vec.size();
}
void BufStream::EnsureCapacity()
{
    if (Length() >= Capacity())
    {
        _vec.resize(3 * Length() / 2);
    }
}
void BufStream::Write(char *bytes, size_t len)
{
    auto res = reinterpret_cast<std::byte *>(bytes);
    Write(res, len);
}
void BufStream::Write(std::byte *bytes, size_t len)
{
    auto end = Length();
    SetLength(end + len);
    EnsureCapacity();
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
void BufStream::SetPos(size_t pos)
{
    _pos = pos;
}
size_t BufStream::Pos() const
{
    return _pos;
}
void BufStream::SetLength(size_t len)
{
    _length = len;
    EnsureCapacity();
}
size_t BufStream::Length()
{
    return _length;
}
bool BufStream::HaveReadableData()
{
    return _length > _pos;
}
