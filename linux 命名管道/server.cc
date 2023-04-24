//服务端

#include"commed.hpp"
//让不同的进程看到同一份资源。
int main()
{
    umask(0);//设置掩码为0；最终权限为(mode & ~umask)
    //创建管道文件，只需要一创建
    int n=mkfifo(fifoname.c_str(),mode);
    //On success mkfifo() returns 0.  In the case of an error, 
    //-1 is returned (in which case, errno is set appropriately).
    if(n != 0)
    {
        std::cout << errno << " : " << strerror(errno) << std::endl;
        return 1;
    }
    sleep(1);
    cout<<"create fifo file sucess" <<std::endl;
    // 2. 让服务端直接开启管道文件
    // open打开命名管道，打开方式是可读可写。返回管道文件的文件描述符。
     int rfd = open(fifoname.c_str(),O_RDWR);
     if(rfd<0)
     {
        std::cout << errno << " : " << strerror(errno) << std::endl;
        return 2;
     }
     sleep(1);
     std::cout << "open fifo success, begin ipc" << std::endl;
     //开始通信
     char buffer[NUM];
     while(1)
     {
        buffer[0]=0;
        //因为从使用系统调用，从管道中读取数据写入数组，必须给字符串预留一个字节，用作存储'\0'
        ssize_t n = read(rfd,buffer,sizeof(buffer)-1);
        if(n>0)//读取到数据
        {
            buffer[n]='\0';
            printf("msg# %s",buffer);
            
        }
        else if(n==0) 
        {
            std::cout << "client quit, me too" << std::endl;
            break;
        }
        else
        {
            std::cout << errno << " : " << strerror(errno) << std::endl;
            break;
        }
     }
    
    //通信结束,关闭不要的fd
    close(rfd);
    unlink(fifoname.c_str());

    return 0;
}
