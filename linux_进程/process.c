#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{

  pid_t pid =fork();
  int count=5;
  if(pid==0)
  {
    while(count--)
    {
      printf("子进程：pid:%d ppid: %d\n",getpid(),getppid());
      sleep(1);
    }
    exit(111);
  }
  else if(pid>0)
  {
    int status;
    waitpid(pid,&status,0);
    printf("父进程：pid:%d ppid :%d,exit code:%d,status:%d",getpid(),getppid(),(status>>8)&0xff,status&0x7f);
   // if(count==0)
   // {
   //  ///exit(111);
   // }
  }


  return 0;
}
