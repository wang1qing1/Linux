#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#define LOG "./log.txt" 

//系统调用
void test3()
{
    //1.打开一个文件,文件还没有创建。//O_WRONLY，O_RDONLY，O_APPEND 他们都是一种宏定义。
    //并且都是位图结构。在第一创建文件的时候，我们还需要给出文件的权限。不然权限都是乱码。
    //并且在给出权限的时候，还要考虑到权限掩码的问题。最终权限 = 起始权限(mask) & ~umask.
    //返回值是一个整数，file descriptor.
    umask(0);
    int fd=open(LOG,O_WRONLY|O_CREAT,0666);
    //2.像文件中写入。
    const char*str="this is a string";
    // ssize_t n = write(fd,str,strlen(str));
    int cur=5;
    char ch[1024];
    while(cur--)
    {
        snprintf(ch,sizeof(ch),"%s:%d\n",str,cur);
        ssize_t n = write(fd,ch,strlen(ch));
    }
    close(fd);
}

void test4()
{
    //当我们第二次打开一个文件的时候，在打开方式(O_WRONLY),是覆盖试的写入。
    int fd=open(LOG,O_WRONLY);
    const char*str="傻逼徐洋洋\n";
    int n=write(fd,str,strlen(str));
    close(fd);
}
void test5()
{
    //要想实现追加试写入，加上一个O_APPEND;
    int fd=open(LOG,O_WRONLY|O_APPEND);
    const char*str="傻逼徐洋洋\n";
    int n=write(fd,str,strlen(str));
    close(fd);
}
int main()
{

    test3();
    test5();
    
    

    return 0;
}
//O_WRONLY:仅写。
//O_RDONLY:仅读。
//O_APPEND:The file is opened in append mode.该文件以追加模式打开。
//O_CREAT:If the file does not exist it will be created.如果该文件不存在，将创建该文件。 