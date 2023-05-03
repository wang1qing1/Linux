#include "myfile.h"

MY_FILE*my_fopen(const char*path,const char*mode)
{
    //1.设置标志位---文件打开方式
    int flag=0;
    if(strcmp(mode,"r")==0) flag|=O_RDONLY;
    else if(strcmp(mode,"w")==0) flag|=(O_CREAT|O_WRONLY|O_TRUNC);
    else if(strcmp(mode,"a")==0) flag|=(O_CREAT|O_WRONLY|O_APPEND);
    else{;}

    //2.打开文件
    umask(0);
    mode_t m=0666; //文件权限
    int fd=0; //文件描述符
    //如果是创建文件，需要调用创建文件的，需要传文件权限；
    if(flag&O_CREAT) fd=open(path,flag,m);
    //打开已经有的文件
    else fd=open(path,flag);

    if(fd<0) return NULL;

    //3.维护MY_FILE给用户返回MY_FILE对象
    MY_FILE*myfd=(MY_FILE*)malloc(sizeof(MY_FILE));
    if(myfd==NULL)//MY_FILE创建失败，也要把打开的文件关上。
    {
        close(fd);
        return NULL;
        //abort();
    }

    //4.初始化MY_FILE对象
    myfd->fd=fd;
    myfd->flags=0;
    myfd->flags|=BUFF_LINE;
    //myfd->outputbuffer[0]={0};
    memset(myfd->outputbuffer,'\0',sizeof(myfd->outputbuffer));
    myfd->current=0;


    return myfd;

    
}