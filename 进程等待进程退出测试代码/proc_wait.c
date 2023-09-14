#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int id = fork();

    if (id == 0) // 子进程
    {
        while (1)
        {
            printf("我是一个子进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
            sleep(1);
        }
    }
    else if (id > 0) // 父进程
    {
        while (1)
        {
            printf("我是一个父进程,我的pid:%d,我的ppid:%d\n", getgid(), getppid());
            sleep(1);
        }
    }

    return 0;
}