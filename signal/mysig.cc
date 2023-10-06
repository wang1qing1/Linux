#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// int main()
// {
//     // 1.创建信号集
//     sigset_t set, old_set;
//     // 2.清空信号集
//     sigemptyset(&set);
//     sigemptyset(&old_set);
//     // 3.添加2号信号到信号集中
//     sigaddset(&set, SIGINT);
//     // 4.将信号集添加进当前进程信号屏蔽字
//     sigprocmask(SIG_BLOCK, &set, &old_set);
//     int count = 0;
//     while (1)
//     {
//         if (count == 7)
//         {
//             // 解除对2信号的屏蔽
//             cout << "解除对2信号的屏蔽,直接递达" << endl;
//             sigprocmask(SIG_UNBLOCK, &set, NULL);
//         }
//         cout << "I am running" << endl;
//         count++;
//         sleep(1);
//     }

//     return 0;
// }

// 打印信号集
void Print_Set_Block()
{
    sigset_t set;
    sigemptyset(&set);

    sigprocmask(SIG_BLOCK, NULL, &set);
    for (int i = 1; i <= 31; i++)
    {
        if (sigismember(&set, i))
            cout << '1';
        else
            cout << '0';
    }
    cout << "\n";
}
// int main()
// {
//     // 1.创建信号集
//     sigset_t set, old_set;
//     // 2.清空信号集
//     sigemptyset(&set);
//     sigemptyset(&old_set);
//     // 3.添加2号信号到信号集中
//     sigaddset(&set, SIGINT);
//     // 4.将信号集添加进当前进程信号屏蔽字
//     cout << "对2信号的屏蔽" << endl;

//     sigprocmask(SIG_BLOCK, &set, &old_set);
//     int count = 0;
//     while (1)
//     {
//         if (count == 7)
//         {
//             // 解除对2信号的屏蔽
//             cout << "解除对2信号的屏蔽,直接递达" << endl;
//             sigprocmask(SIG_UNBLOCK, &set, NULL);
//         }
//         cout << "I am running" << endl;
//         Print_Set_Block();
//         count++;
//         sleep(1);
//     }

//     return 0;
// }

// 打印信号集
// void Print_Set_Pend()
// {
//     sigset_t set;
//     sigemptyset(&set);

//     sigpending(&set);
//     cout << "pending表:";
//     for (int i = 1; i <= 31; i++)
//     {
//         if (sigismember(&set, i))
//             cout << '1';
//         else
//             cout << '0';
//     }
//     cout << "\n";
// }
// void handler(int signo)
// {
//     cout << "收到信号:" << signo << endl;
// }
// int main()
// {

//     // 0.自定义捕捉2号信号
//     signal(2, handler);

//     // 1.创建信号集
//     sigset_t set, old_set;
//     // 2.清空信号集
//     sigemptyset(&set);
//     sigemptyset(&old_set);
//     // 3.添加2号信号到信号集中
//     sigaddset(&set, SIGINT);
//     // 4.将信号集添加进当前进程信号屏蔽字
//     cout << "对2信号屏蔽10秒" << endl;

//     sigprocmask(SIG_BLOCK, &set, &old_set);
//     int count = 0;
//     while (1)
//     {
//         if (count == 10)
//         {
//             // 解除对2信号的屏蔽
//             cout << "解除对2信号的屏蔽,直接递达" << endl;
//             sigprocmask(SIG_UNBLOCK, &set, NULL);
//         }
//         // cout << "I am running" << endl;
//         Print_Set_Pend();
//         count++;
//         sleep(1);
//     }

//     return 0;
// }

// void handler(int signo)
// {
//     while (1)
//     {
//         cout << "收到信号:" << signo << endl;
//         sleep(2);
//     }
// }

// int main()
// {

//     // 1.创建sigaction结构体
//     struct sigaction sigc, oldsigc;
//     memset(&sigc, 0, sizeof(sigc));
//     memset(&sigc, 0, sizeof(oldsigc));

//     // 2.填写成员
//     // 2.1自定义捕捉
//     sigc.sa_handler = handler;
//     // 2.2在执行自定义捕捉函数时，想要屏蔽的信号,屏蔽2.3.4.5.6信号
//     sigset_t set;
//     sigemptyset(&set);
//     sigaddset(&set, 3);
//     sigaddset(&set, 4);
//     sigaddset(&set, 5);
//     sigaddset(&set, 6);
//     sigc.sa_mask = set;
//     // 3.写入进程
//     sigaction(2, &sigc, &oldsigc);

//     while (1)
//     {
//         cout << "I am running,pid:" << getpid() << endl;
//         sleep(1);
//     }

//     return 0;
// }

// void handler(int signo)
// {
//     sleep(3);
//     int status = 0;
//     waitpid(-1, &status, WNOHANG);
// }

int main()
{
    // 1.创建sigaction结构体
    struct sigaction sigc;
    memset(&sigc, 0, sizeof(sigc));
    // 2.设置sa_handler为SIG_IGN;
    sigc.sa_handler = SIG_IGN;
    // 写入当前进程
    sigaction(SIGCHLD, &sigc, NULL);
    int tmp = 10;
    while (tmp--)
    {
        //  创建子进程
        pid_t pid = fork();
        if (pid == 0)
        {
            int count = 3;
            while (count)
            {
                cout << "我是子进程,pid:" << getpid() << ":" << count << endl;
                sleep(1);
                count--;
            }
            exit(2);
        }
    }

    while (1)
    {
        cout << "父进程" << endl;
        sleep(5);
    }

    return 0;
}