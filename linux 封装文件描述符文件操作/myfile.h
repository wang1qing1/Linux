#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define NUM 1024
#define BUFF_NONE 1
#define BUFF_LINE 2
#define BUFF_ALL 3

typedef struct _MY_FILE
{
    int fd;                 // 文件描述符
    int flags;              // 文件刷新方式
    char outputbuffer[NUM]; // 缓冲区
    int current;            // 当前写入位置
} MYFILE;

MYFILE *my_fopen(const char *path, const char *mode);
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MYFILE *stream);
void my_fclose(MYFILE *fp);
void my_fflush(MYFILE *fp);
