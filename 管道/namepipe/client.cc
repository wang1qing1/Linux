#include "Comm.hpp"

int main()
{
    // 1.打开管道
    int pipefd = open(namepipe, O_RDONLY);

    // 2.开始通信，接受信息
    while (1)
    {
        int quit = 0;
        char buff[1024] = {0};
        // size_t n = read(pipefd, buff, sizeof(buff));
        size_t n = read(pipefd, buff, sizeof(char));
        if (n == 0)
            continue;
        buff[n] = 0;
        printf("%c", buff[0]);
        fflush(stdout);
        if (strcmp(buff, "quit") == 0)
        {
            break;
        }
    }

    return 0;
}