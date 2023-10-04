

using namespace std;

// int main()
// {

//     // 创建子进程
//     pid_t pid = fork();
//     if (pid == 0)
//     {
//         // 让子进程捕捉2号信号
//         signal(2, handle);

//         while (1)
//         {
//             printf("I am child process, my PID :%d,I am waiting signal!\n", getpid());
//             sleep(1);
//         }
//     }

//     int count = 5;
//     while (count)
//     {
//         printf("%d秒之后我将给%d号进程发2号信号\n", count--, pid);
//         sleep(1);
//     }
//     // 给子进程发送2号信号
//     kill(pid, 2);
//     sleep(1);
//     return 0;
// }

// void handle(int signum)
// {
//     cout << "I am process,I get a signal:" << signum << endl;
//     // 进程退出
//     exit(0);
// }

// int main()
// {

//     // 让进程捕捉6号信号
//     // signal(6, handle);

//     int count = 5;
//     while (count)
//     {
//         printf("%d秒之后我将给自己发6号信号\n", count--);
//         sleep(1);
//     }
//     // 给子进程发送6号信号
//     abort();
//     sleep(15);
//     return 0;
// }

// void handle(int signum)
// {
//     cout << "I am process,I get a signal:" << signum << endl;
//     // 进程退出
//     exit(0);
// }

// int main()
// {

//     // 设置闹钟
//     alarm(5);
//     int count = 5;
//     while (count)
//     {
//         printf("%d秒之后闹钟响,进程终止\n", count--);
//         sleep(1);
//     }
//     sleep(15);
//     return 0;
// }

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

void handle(int signum)
{
    cout << "I am process,I get a signal:" << signum << endl;
    // 进程退出
    exit(0);
}

int main()
{

    pid_t pid = fork();
    if (pid == 0)
    {
        cout << "begin" << endl;
        cout << "begin" << endl;
        int *point = nullptr;
        *point = 100;

        cout << "end" << endl;
        cout << "end" << endl;
    }

    int status = 0;
    waitpid(pid, &status, 0);
    cout << "core dump:" << ((status >> 7) & 1) << endl;
    cout << "signal:" << (status & 0x3f) << endl;

    return 0;
}