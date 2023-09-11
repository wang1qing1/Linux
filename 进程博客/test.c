#include <stdio.h>
#include <unistd.h>

int main()
{
    while (1)
    {
        printf("我是一个进程,我的pid:%d\n", getpid());
        sleep(1);
    }

    return 0;
}