#include "utility.h"
#include <unistd.h>
using namespace squid;
void CommonUtility::CloseFd(int &fd)
{
    if (fd < 0)
        return;
    close(fd);
    fd = -1;
}