#include"commed.hpp"

int main()
{
    //客户端无需创建管带==管道文件，我们只需打开已经创建的管道文件就可以了
    int wfd=open(fifoname.c_str(),O_RDWR);
    if(wfd<0)
    {
        cout<<errno<<":"<<strerror(errno)<<endl;
        return 1;
    }
    
    //可以进行通信了
    char buffer[NUM];
    while(1)
    {
        cout<<"请输入你的请求# ";
        //从显示器上读取数据，写入buffer，读取sizeof(buffer)个字节;
        char *msg=fgets(buffer,sizeof(buffer),stdin);
        assert(msg);
        (void)msg;
        
        ssize_t n=write(wfd,buffer,sizeof(buffer));
        assert(n>=0);
        (void)n;
    }

    return 0;
}