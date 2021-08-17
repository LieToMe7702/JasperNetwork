#pragma once
#include "fmt/format.h"
#include <string>
#include <fstream>
namespace squid
{
class Logger
{
    public:
    void Init(std::string path);
    void Info(std::string str);
    void Debug(std::string str);
    void Error(std::string str);

  private:
    std::ifstream file;
    ~Logger();
};
} // namespace squid
