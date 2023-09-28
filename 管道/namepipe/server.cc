#include "Comm.hpp"

int main()
{

    // 1.创建命名管道namepipe
    int ret = mkfifo(namepipe, mode);
    if (ret == -1)
    {
        cerr << errno << ":" << strerror(errno) << endl;
    }

    // 2.打开管道
    int pipefd = open(namepipe, O_WRONLY);
    if (pipefd == -1)
    {
        cerr << errno << ":" << strerror(errno) << endl;
    }

    // // 3.开始通信
    // while (1)
    // {
    //     int quit = 0;
    //     char buff[1024] = {0};
    //     char *str = gets(buff);
    //     if (strcmp(str, "quit") == 0)
    //     {
    //         quit = 1;
    //     }
    //     // buff[strlen(str)] = '\n';
    //     size_t n = write(pipefd, buff, strlen(str));
    //     if (quit)
    //     {
    //         break;
    //     }
    // }

    // 3.开始通信,即使传达，无需回车
    while (1)
    {

        system("stty raw");
        int c = getchar();
        system("stty -raw");

        // buff[strlen(str)] = '\n';
        size_t n = write(pipefd, (char *)&(c), sizeof(char));
    }

    return 0;
}
