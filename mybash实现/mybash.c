#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

void showenv()
{
    extern char **environ;
    for (int i = 0; environ[i]; i++)
    {
        printf("[%d]:%s\n", i, environ[i]);
    }
}

int main()
{
    int status = 0;
    char myenv[32][512] = {0};
    int index_env = 0;
    int last_exit = 0;
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

        // 注意：cd,export,env,echo,等命令都是内建命令，即这些命令不能创建子进程执行，只能bash自己执行
        if (strcmp(subbuff[0], "cd") == 0)
        {
            if (subbuff[1] != NULL)
                chdir(subbuff[1]);

            continue;
        }
        else if (strcmp(subbuff[0], "export") == 0)
        {
            if (subbuff[1] != NULL)
            {
                // 这里不能将subbuff[1]直接导入环境变量，因为环境变量表存储的都是指针，必须使用一个单独空间
                strcpy(myenv[index_env], subbuff[1]);
                putenv(myenv[index_env++]);
            }
            continue;
        }
        else if (strcmp(subbuff[0], "env") == 0)
        {
            showenv();
            continue;
        }
        else if (strcmp(subbuff[0], "echo") == 0)
        {
            // echo $PATH
            if (subbuff[1][0] == '$')
            {
                if (subbuff[1][1] == '?') // echo $?//最近一次推出吗
                {
                    printf("%d\n", last_exit);
                }
                else // 提取环境变量
                {
                    // PATH
                    char *subenv = subbuff[1] + 1;
                    char *get_env = getenv(subenv);
                    if (get_env != NULL)
                    {
                        printf("%s=%s\n", subenv, get_env);
                    }
                }
            }
            continue;
        }
        if (strcmp(subbuff[0], "ls") == 0)
        {
            int comm_index = 0;
            while (subbuff[comm_index])
            {
                comm_index++;
            }
            // 增加高亮
            subbuff[comm_index] = "--color=auto";
        }

        // 创建子进程
        pid_t pid = fork();
        assert(pid >= 0);

        if (pid == 0) // 子进程
        {
            execvp(subbuff[0], subbuff);
        }

        // // 测试
        // debug(subbuff);

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            last_exit = WEXITSTATUS(status);
        }
    }

    return 0;
}
