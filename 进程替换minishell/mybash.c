#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

#define SEP " "
#define MAX 1024
#define MAX_SUB 64

void split(char buff[], char *subbuff[])
{
    assert(buff);
    assert(subbuff);

    int i = 0;
    subbuff[i++] = strtok(buff, SEP);
    while (subbuff[i++] = strtok(NULL, SEP)) // 如果没有找到分割，就会返回NULL
        ;
}

void debug(char *subbuff[])
{
    int i = 0;
    while (subbuff[i])
    {
        printf("%s\n", subbuff[i++]);
    }
}

int main()
{

    while (1)
    {
        char buff[MAX] = {0}; // 存储命令行输入的命令字符串
        char *subbuff[MAX_SUB] = {NULL};
        printf("wq@[aliyum]$:");
        fflush(stdout);
        // 获得命令字符串
        fgets(buff, sizeof(buff), stdin);

        // ls -a -l\n\0,strlen=9,index(\n)=8,去除回车。
        buff[strlen(buff) - 1] = '\0';

        // 分割字符串
        split(buff, subbuff);

        // 创建子进程
        pid_t pid = fork();
        assert(pid >= 0);

        if (pid == 0) // 子进程
        {
            execvp(subbuff[0], subbuff);
        }

        // // 测试
        // debug(subbuff);

        wait(-1);
    }

    return 0;
}
