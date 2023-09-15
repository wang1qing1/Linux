#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int status;
    pid_t id = fork();

    if (id == 0) // 子进程
    {
        int n = 5;
        while (n--)
        {
            printf("我是一个子进程,我的pid:%d,我的ppid:%d\n", getpid(), getppid());
            sleep(1);
        }
        return 100;
    }
    else if (id > 0) // 父进程
    {
        int n = 10;
        while (n--)
        {
            pid_t ret = waitpid(-1, &status, WNOHANG);
            if (ret == 0)
            {
                printf("子进程还未结束\n");
            }
            else // ret > 0
            {
                if (WIFEXITED(status)) // 提取后七位结果为0,没有收到信号，正常退出。
                {
                    // printf("子进程正常结束!!!子进程pid:%d,退出码:%d\n", ret, (status >> 8) & 0x7F);
                    printf("子进程正常结束!!!子进程pid:%d,退出码:%d\n", ret, WEXITSTATUS(status));
                }
                else
                {
                    printf("子进程异常退出子进程pid:%d,收到信号:%d\n", ret, (status) & 0x3F);
                }
                sleep(1);
                exit(0);
            }
            sleep(1);
            printf("我是一个父进程,我的pid:%d,我的ppid:%d\n", getpid(), getppid());
        }
    }
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int main()
// {
//     int id = fork();

//     if (id == 0) // 子进程
//     {
//         int n = 5;
//         while (n--)
//         {
//             printf("我是一个子进程,我的pid:%d,我的ppid:%d\n", getpid(), getppid());
//             sleep(1);
//         }
//         return 0;
//     }
//     else if (id > 0) // 父进程
//     {
//         wait(NULL); // 子进程一旦退出就立马回收
//         int n = 10;
//         while (n--)
//         {
//             printf("我是一个父进程,我的pid:%d,我的ppid:%d\n", getpid(), getppid());
//             sleep(1);
//         }
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int main()
// {
//     int id = fork();

//     if (id == 0) // 子进程
//     {
//         int n = 5;
//         while (n--)
//         {
//             printf("我是一个子进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
//             sleep(1);
//         }
//         return 0;
//     }
//     else if (id > 0) // 父进程
//     {
//         int n = 10;
//         while (n--)
//         {
//             printf("我是一个父进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
//             sleep(1);
//         }
//     }
//     wait(NULL);
//     return 0;
// }

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>

// int main()
// {
//     int status;
//     int id = fork();

//     if (id == 0) // 子进程
//     {
//         int n = 5;
//         while (n--)
//         {
//             printf("我是一个子进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
//             sleep(1);
//         }
//         return 0;
//     }
//     else if (id > 0) // 父进程
//     {
//         waitpid(-1, &status, 0);
//         int n = 10;
//         while (n--)
//         {
//             printf("我是一个父进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
//             sleep(1);
//         }
//     }
//     return 0;
// }