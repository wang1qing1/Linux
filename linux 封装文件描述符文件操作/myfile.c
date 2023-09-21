#include "myfile.h"

// path:文件名称，mode:打开方式
MYFILE *my_fopen(const char *path, const char *mode)
{
    // 1.读取打开方式
    int flags = 0;
    if (strcmp(mode, "w") == 0)
    {
        flags |= (O_CREAT | O_WRONLY | O_TRUNC);
    }
    else if (strcmp(mode, "r") == 0)
    {
        flags |= O_RDONLY;
    }
    else if (strcmp(mode, "a") == 0)
    {
        flags |= (O_CREAT | O_WRONLY | O_APPEND);
    }
    else
    {
        ;
    }

    mode_t m = 0666;
    int fd = 0;

    // 2.打开文件
    if (flags & O_CREAT)
    {
        fd = open(path, flags, m);
    }
    else
    {
        fd = open(path, flags);
    }

    if (fd < 0)
    {
        return NULL;
    }

    // 3.创建MYFILE对象
    MYFILE *myfile = (MYFILE *)malloc(sizeof(MYFILE));
    if (myfile == NULL)
    {
        close(fd);
        return NULL;
    }

    // 初始化MYFILE
    myfile->fd = fd;
    myfile->flags = 2;

    myfile->current = 0;
    memset(myfile->outputbuffer, '\0', NUM);

    // 返回MYFILE对象指针
    return myfile;
}

void my_fflush(MYFILE *stream)
{
    assert(stream);
    write(stream->fd, stream->outputbuffer, stream->current);
    stream->current = 0;

    fsync(stream->fd);
}

void my_fclose(MYFILE *stream)
{
    assert(stream);
    // 如果缓冲区有数据，刷新缓冲区
    if (stream->current > 0)
    {
        my_fflush(stream);
    }
    // 关闭文件描述符
    close(stream->fd);
    // 释放文件结构体空间
    free(stream);
}

// ptr:写入的数据，size:一次写入的大小，nmemb:写入的次数，stream:写入的文件
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MYFILE *stream)
{

    // 1.如果缓冲区已经满了，就先将数据刷新出去
    if (stream->current == NUM)
        my_fflush(stream);

    // 2.根据缓冲区的剩余情况，进行数据拷贝
    size_t user_size = size * nmemb;
    size_t my_size = NUM - stream->current;

    size_t write_size = 0;
    // 缓冲区剩余空间充足
    if (my_size >= user_size)
    {
        memcpy(stream->outputbuffer + stream->current, ptr, user_size);
        stream->current += user_size;
        write_size += user_size;
    }
    else // 缓冲区剩余空间不充足
    {
        memcpy(stream->outputbuffer + stream->current, ptr, my_size);
        stream->current += my_size;
        write_size += my_size;
    }

    // 4.计划刷新
    if (stream->flags == BUFF_ALL)
    {
        if (stream->current == NUM)
            my_fflush(stream);
    }
    else if (stream->flags == BUFF_LINE)
    {
        if (stream->outputbuffer[stream->current - 1] == '\n')
            my_fflush(stream);
    }
    else
    {
        ;
    }
    return write_size;
}
