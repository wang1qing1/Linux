#include<stdio.h>
#include<unistd.h>
int main()
{
  pid_t ret = fork();
  while(1)
  {  
    if(ret==0)
    {

      printf("我是一个子进程pid:%d,ppid:%d\n",getpid(),getppid());
    }
     else if(ret > 0)
    {
      printf("我是一个父进程pid:%d,ppid:%d\n",getpid(),getppid());
    }
  }
  return 0;
}
