#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include "Log.hpp"
using namespace std;

struct Util
{
    static bool Noblock(int fd)
    {
        // 获得文件状态
        int fl = fcntl(fd, F_GETFD);
        if (fl < 0)
        {
            Logmessage(Warning, "Set Noblock Fail...");
            return false;
        }
        // 文件状态添加非阻塞
        fcntl(fd, F_SETFL, fl | O_NONBLOCK);
        return true;
    }
};
