#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#define LOG "./log.txt" 
//语言层接口
void test1()
{
    FILE*fid=fopen(LOG,"a");
    const char *str="this is a string";
    int cur=10;
    while(cur)
    {
        //接口1：格式化向文件中写入内容,也可以向显示器文件(stdout)写入.
        //fprintf(fid,"%s:%d\n",str,cur);
        fprintf(fid,"%s:%d\n",str,cur);
        cur--;
    }   
}

void test2()
{
    const char *str="this is a string";
    int cur=10;
    char msg[1024]={0};
    //接口2:格式化向数组中写入内容
    snprintf(msg,sizeof(msg),"%s:%d\n",str,cur);
    printf("%s",msg);
}


int main()
{
    // test1();
    // test1();

    
    

    return 0;
}
