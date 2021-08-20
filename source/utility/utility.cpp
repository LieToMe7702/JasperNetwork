#include "utility.h"
#include <unistd.h>
using namespace squid;
void Utility::CloseFd(int &fd)
{
    if (fd < 0)
        return;
    close(fd);
    fd = -1;
}