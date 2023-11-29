
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "Log.hpp"
#include "server.hpp"

using namespace std;

void NoBlock(int fd)
{
    // 获得/设置文件状态标记
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        Logmessage(Error, "errno %d,%s", strerror(errno));
        return;
    }
    // 设置fd的读取位非阻塞
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

int main()
{
    Server tcpserver(8081);
    tcpserver.start();
}
