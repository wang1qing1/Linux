#include<iostream>
#include<string>
#include<cstring>
#include <cerrno>
#include<cstdio>
#include <cassert>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

using namespace std;

#define SERVER 1
#define CLIENT 0
#define PATHNAME "."//路径字符串
#define PROJID 0x6666//项目id
const int gsize=4096; //共享内存大小



// key_t getkey()
// {

//     //返回一个key值，参数一：创建路径字符串，参数二，项目id
//     //key是一个操作系统级别的对共享内存的标识。
//     key_t k = ftok(PATHNAME,PROJID);
//     if(k==-1)
//     {
//         cerr<<"error: "<<errno<<":"<<strerror(errno)<<endl;
//         exit(1);
//     }
//     return k;
// }

// int createShmHelper(key_t k,int size,int flag)
// {
//     //shmget通过k，去创建shm，大小通过gsiz来控制，调用失败会设置错误码
//     //创建的规则有flag来控制，
//     //IPC_CREAT | IPC_EXCL 创建一个共享内存段，如果已经存在，就直接返回-1
//     //所以创建成功以后得到的一定是一个全新的共享内存，并返回其shmid(共享内存用户级别标识符)
//     //IPC_CREAT 创建一个没有就创建一个新的，如果已经存在，就返回已经存在的共享内存shmid
//     int shmid =shmget(k,size,flag);
//     if(shmid==-1)
//     {
//         cerr << "error: " << errno << " : " << strerror(errno) << endl;
//         exit(2);
//     }
//     return shmid;
// }

// //创建用户级别的共享内存的标识符shmid，对于服务端需要创建shmid，
// int createShm(key_t k,int size)
// {
//     umask(0);//权限掩码
//     //共享内存权限666；
//     return createShmHelper(k,size,IPC_CREAT|IPC_EXCL|0666);
// }

// //对于用户端需要找到我们已经创建好的shm。
// int getShm(key_t k,int size)
// {
//     //IPC_CREAT 创建一个没有就创建一个新的，如果已经存在，就返回已经存在的共享内存shmid
//     return createShmHelper(k,size,IPC_CREAT);
// }

// //将共享内存与服务端进程，用户端进程挂接
// char* attachShm(int shmid)
// {
//     //shmat（）将shmid标识的System V共享内存段附加到调用进程的地址空间。
//     //成功返回共享内存的首地址，失败返回(void*)-1。
//     char*start=(char*)shmat(shmid,nullptr,0);
//     if(start==-1)
//     {
//         cout<<erron<<":"<<strerror(errno)<<endl;
//     }
//     return start;
// }

// //将共享内存与进程去链接
// void detachShm(char*start)
// {
//     //shmdt() 从调用进程的地址空间中分离位于start指向的地址处的共享内存段。
//     //要分离的段当前必须挂接shmat () 调用返回的值的start
//     int n=shmdt(start);
//     assert(n!=-1);
//     (void)n;
// }

// //删除共享内存段。
// void delShm(int shmid)
// {
//     //标记要销毁的分段。只有在最后一个进程将该段分离后
//     //即最后一个与改该共享内存挂接的进程被去关联时，该共享内存才会被销毁。
//     //调用必须有对该共享内存的删除权限。
//     int n= shmctl(shmid,IPC_RMID,nullptr);
//     assert(n!=-1)
//     (void)n;
// }

class Shm
{
public:
    
    Shm(int flag)
        :_flag(flag)
    {
        if(flag==SERVER)
        {
            shmid=createShm(getkey(),4096);
        }
        else 
        {
            shmid=getShm(getkey(),4096);
        }
        start=attachShm();
    }
    char* getstart()
    {
        return start;
    }
    ~Shm()
    {
        detachShm();
        delShm();
    }
private:
    //将共享内存与进程去链接
    void detachShm()
    {
        //shmdt() 从调用进程的地址空间中分离位于start指向的地址处的共享内存段。
        //要分离的段当前必须挂接shmat () 调用返回的值的start
        int n=shmdt(start);
        assert(n!=-1);
        
    }

    //删除共享内存段。
    void delShm()
    {
        //标记要销毁的分段。只有在最后一个进程将该段分离后
        //即最后一个与改该共享内存挂接的进程被去关联时，该共享内存才会被销毁。
        //调用必须有对该共享内存的删除权限。
        int n= shmctl(shmid,IPC_RMID,nullptr);
        assert(n!=-1);
    }

    //将共享内存与服务端进程，用户端进程挂接
    char* attachShm()
    {
        //shmat（）将shmid标识的System V共享内存段附加到调用进程的地址空间。
        //成功返回共享内存的首地址，失败返回(void*)-1。
        char*start=(char*)shmat(shmid,nullptr,0);
        if(start==(char*)-1)
        {
            cout<<errno<<":"<<strerror(errno)<<endl;
        }
        return start;
    }

    key_t getkey()
    {
        //返回一个key值，参数一：创建路径字符串，参数二，项目id
        //key是一个操作系统级别的对共享内存的标识。
        key_t k = ftok(PATHNAME,PROJID);
        if(k==-1)
        {
            cerr<<"error: "<<errno<<":"<<strerror(errno)<<endl;
            exit(1);
        }
        return k;
    }

    //对于用户端需要找到我们已经创建好的shm。
    int getShm(key_t k,int size)
    {
        //IPC_CREAT 创建一个没有就创建一个新的，如果已经存在，就返回已经存在的共享内存shmid
        return createShmHelper(k,size,IPC_CREAT);
    }

    //创建用户级别的共享内存的标识符shmid，对于服务端需要创建shmid，
    int createShm(key_t k,int size)
    {
        umask(0);//权限掩码
        //共享内存权限666；
        return createShmHelper(k,size,IPC_CREAT|IPC_EXCL|0666);
    }

    int createShmHelper(key_t k,int size,int flag)
    {
        //shmget通过k，去创建shm，大小通过gsiz来控制，调用失败会设置错误码
        //创建的规则有flag来控制，
        //IPC_CREAT | IPC_EXCL 创建一个共享内存段，如果已经存在，就直接返回-1
        //所以创建成功以后得到的一定是一个全新的共享内存，并返回其shmid(共享内存用户级别标识符)
        //IPC_CREAT 创建一个没有就创建一个新的，如果已经存在，就返回已经存在的共享内存shmid
        int shmid =shmget(k,size,flag);
        if(shmid==-1)
        {
            cerr << "error: " << errno << " : " << strerror(errno) << endl;
            exit(2);
        }
        return shmid;
    }
    int _flag;
    int shmid;
    char *start;
};







