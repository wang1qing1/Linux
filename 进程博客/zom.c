#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid > 0) // parent
    {
        printf("父进程 begin -30秒\n");
        sleep(30);
        exit(0);
    }
    else if (pid == 0)
    {
        printf("子进程 begin -5秒\n");
        sleep(5);
        exit(0);
    }

    return 0;
}