#include<iostream>
#include<signal.h>
#include<unistd.h>
using namespace std;



//自定义方法
//signo：特定信号被发送给当前进程的时候，执行handler方法的时候，
//要自动填充对应的信号给handler方法
void handler(int num)
{
    cout<<"the signal is "<<num<<endl;
}

int main()
{
    //signal(),是一个用作捕捉信号的系统调用。
    //参数一:需要捕捉的信号，对捕捉的信号做自定义处理。
    //参数二:自定义处理的函数。typedef void (*sighandler_t)(int);
    //注意：signal(2, handler)调用完这个函数的时候，handler方法被调用了吗？
    //没有！ 做了什么？只是更改了2号信号的处理动作，并没有调动handlder方法
    //当信号被写入当前进程的时候，才会调用
    //2号信号默认是，终止进程，当我们通过命令给进程发信号时，就不会在执行默认动作，
    //而是执行自定义的函数。
    signal(2,handler);
    while(1)
    {
        cout<<"我是一个进程"<<endl;
        sleep(1);
    }
    return 0;
    
}


