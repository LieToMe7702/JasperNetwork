#pragma once
#include "fmt/core.h"
#include "fmt/printf.h"
namespace squid
{
class Utility
{
  public:
    static void CloseFd(int &fd);
};
} // namespace squid