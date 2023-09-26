#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include "Task.hpp"

using namespace std;
#define NUM_PROC 5

struct child_pip
{
    child_pip(int fd, pid_t pid)
        : _fd(fd), _pid(pid)
    {
    }
    ~child_pip()
    {
    }

    int _fd;
    pid_t _pid;
};

void WaitCommand()
{
    Task task;
    int command;
    while (1)
    {
        size_t n = read(0, &command, sizeof(int));
        command %= task.funcs.size();
        if (n == 4) // 读取成功
        {
            task.funcs[command]();
        }
        else if (n == 0) // 读取失败
        {
            break;
        }
        else
        {
            break;
        }
    }
}
void creatproc(vector<child_pip> &child_pip_v)
{
    for (int i = 0; i < NUM_PROC; i++)
    {
        // 1.创建管道
        int pipfd[2];
        pipe(pipfd);

        // 2.创建子进程
        pid_t pid = fork();
        if (pid < 0)
            perror("fork");

        // 我们想让子进程从管道读，父进程向管道写
        if (pid == 0) // 子进程
        {
            // 3.关闭不必要的文件描述符
            close(pipfd[1]);
            // 3.1重定向，将来子进程指向0号文件描述符读取
            dup2(pipfd[0], 0);

            WaitCommand();

            exit(0);
        }
        // 父进程
        //  3.关闭不必要的文件描述符
        close(pipfd[0]);

        // 建立好子进程与管道的映射,父进程的写端口，和子进程pid
        child_pip_v.push_back(child_pip(pipfd[1], pid));
    }
}

int main()
{

    vector<child_pip> child_pip_v;
    creatproc(child_pip_v);

    while (1)
    {
        int command = 0;
        cin >> command;
        int i = rand() % NUM_PROC;
        write(child_pip_v[i]._fd, &command, sizeof(int));
    }

    return 0;
}
