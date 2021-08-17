#include "logger.h"
#include <ios>
void squid::Logger::Init(std::string path)
{
    file.open(path,std::ios::in);
}
squid::Logger::~Logger()
{
    if (file)
    {
        file.close();
    }
}
