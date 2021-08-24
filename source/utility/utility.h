#pragma once
#include "fmt/core.h"
#include "fmt/printf.h"
#include <algorithm>
#include <functional>
#include <vector>
namespace squid
{
class CommonUtility
{
  public:
    static void CloseFd(int &fd);
};
} // namespace squid