
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
    NoBlock(0);
    char buff[64];
    while (1)
    {
        int n = read(0, buff, sizeof(buff));
        if (n < 0)
        {
            Logmessage(Debug, "read is not really,errno:%d,%s", errno, strerror(errno));
            sleep(1);
            continue;
        }
        buff[n - 1] = 0;
        cout << "echo>>>" << buff << endl;
        sleep(1);
    }
}
