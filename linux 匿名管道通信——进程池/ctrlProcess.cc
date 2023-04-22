#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<fcntl.h>
#include<cassert>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

//约定，每一个command都必须是4字节
#define COMMAND_LOG 0
#define COMMAND_MYSQL 1
#define COMMAND_REQEUST 2

void PrintLog()
{
    cout<<"pid:"<<getpid()<<",打印日志任务，正在被执行..."<<endl;
}
void InsertMySQL()
{
    cout<<"pid:"<<getpid()<<",执行数据库任务，正在被执行..."<<endl;
}

void NetRequest()
{
    cout<<"pid:"<<getpid()<<"执行网络请求任务，正在被执行..." << endl;
}

class Task
{
    public:
    Task()
    {
        v_fun.push_back(PrintLog);
        v_fun.push_back(InsertMySQL);
        v_fun.push_back(NetRequest);
    }
    void Execute(int command)
    {
        if(command>=0&&command<v_fun.size())
        {
            v_fun[command]();
        }
    }
    vector<void(*)()> v_fun;
};

class EndPoint
{
public:
    pid_t _child_pid;
    size_t _write_fd;
    EndPoint(pid_t id,size_t fd)
    :_child_pid(id),_write_fd(fd)
    {}
    ~EndPoint()
    {

    }
};

 void WaitCommand()
 {
    while(1)
    {
        Task t;
        int command=0;
        //子进程一直处于等待管道中的数据的阻塞状态。
        //如果read读取完了管道中的所有数据，写入端不写入，子进程就会一直等待。
        int n=read(0,&command,sizeof(command));
        if(n==sizeof(command))
        {
            t.Execute(command);
        }
        else if(n==0)
        {
            break;
        }
        else
        {
            break;
        }
    }
 }
const int Pnum=5;
void CreateProcess( vector<EndPoint>& v_process)
{
    for(int i=0;i<Pnum;i++)
    {
        //创建管道
        //创建了一个管道，并且将filefd数组的内容填充
        int filefd[2]={0};
        int n=pipe(filefd);
        //管道创建成功，就返回0，保证管道一定创建成功
        assert(n==0);
        //assert()在发布版本会被去掉
        //防止出现变量未使用的情况
        //void*(n);

        //创建子进程
        pid_t pid = fork();//给父进程返回子进程pid，给子进程返回0
        assert(pid!=-1);//创建失败返回-1
        
        //子进程执行
        if(pid==0)
        {
            //因为创建父子进程和匿名管道关系，父进程创建了管道。
            //子进程会继承父进程的关系，在子进程中filefd的3，4号文件描述
            //关闭子进程不需要的文件描述符
            close(filefd[1]);//我们期望子进程读取
            //而管道通信，关闭不必要的文件描述符，我们期望子进程读取“指令”，
            //都从标准输入读取,子进程的标准输入文件重定向到，管道的读端口。
            //0---stdin
            //1---stdout
            //2---stderr
            dup2(filefd[0],0);
            WaitCommand();
            //子进程完成读取任务，关闭读端文件描述符
            close(filefd[0]);
            //完成任务退出
            exit(0);
        }
        
        //此位置一定是父进程
        //1.3关闭父进程不用的文件描述符
        close(filefd[0]);
        //将新的子进程和他的管道写端，构建对象
        v_process.push_back(EndPoint(pid,filefd[1]));
    }
}

void WaitProcess(vector<EndPoint>& v_process)
{
    //在子进程完成其指定的任务以后，由于子进程仍正在等待，让子进程正常退出
    //第一步：首先让子进程不在处于等待管道的阻塞状态。即，关闭子进程对应管道的写端。
    for(auto &e:v_process)
    {
        close(e._write_fd);
        cout<<"管道写入端已经关闭"<<"pid:"<<e._child_pid<<endl;
    }
    sleep(5);
    //第二步：回收子进程的僵尸状态
    for(auto &e:v_process)
    {
        cout<<"父进程回收了子进程的僵尸状态"<<e._child_pid<<endl;
        waitpid(e._child_pid,nullptr,0);
    }

    //第一个坑，这里的循环是分开写的，因为当当父进程创建第二个管道和子进程的时候，
    //由于子进程会继承父进程对管道的读写端，所以子进程不仅会继承，父进程的对于本次创建的管道的读写端，还会继承前面
    //几次父进程的对于其他管道的写端的文件描述符。
    //如果将循环一起写，也就是我们关闭一次写端，就去回收，这个时候，由于上述问题，我们以为只有一个父进程有对管道的写端，我们只是关闭了父进程的写端，
    //但是实际上后面出现的子进程都会有对前面管道的写端。
    //所以此时的管道并没有完全关闭所有写端，子进程仍会处于，等待管道写入的阻塞状态，也就没有办法退出回收了。

    //解决方案 1.分两个循环来写，分别处理，关闭写端口和回收子进程的僵尸状态。
    //        2.一个循环，从后往前关闭管道读端口，和回收子进程的僵尸状态。
    //        3.一个循环，正常关闭和回收，但是在创建子进程和管道的时候，每次创建子进程前，先关闭对前面管道的写端
}
int main()
{
    //将来我们通过父进程向管道写入"指令"，来控制子进程完成某些工作.
    //所我们创建一个对象描述我们维护的管道和其匹配的子进程。
    //子进程pid和子进程对应管道的写入端。
    vector<EndPoint> v_process;

    //创建子进程和对应的匿名管道
    CreateProcess(v_process);
    int count=5;
    while(count--)
    {
        //选择任务
        int command = COMMAND_LOG;
        //选择进程
        int index=rand()%v_process.size();

        //指派任务
        write(v_process[index]._write_fd,&command,sizeof(command));
        sleep(1);
    }
    WaitProcess(v_process);

    return 0;
}

