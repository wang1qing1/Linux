#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define NUM 1024
#define BUFF_LINE  0x1
#define BUFF_NONE  0x2
#define BUFF_ALL   0x4

typedef struct MY_FILE
{
    int fd;//文件描述符
    int flags;//刷新方式
    char outputbuffer[NUM];//缓冲区
    int current;//当前缓冲区的字节数
}MY_FILE;

MY_FILE*my_fopen(const char*path,const char*mode);

