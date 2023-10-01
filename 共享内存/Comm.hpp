#include <iostream>
#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

const char *pathname = "/home/wq/23_10_1";
const int pro_id = 0666;
const size_t gsize = 4096;

#define CLIENT 1
#define SERVER 0

using namespace std;
class Shm
{
public:
    Shm(int type)
        : _type(type)
    {
        getkey();
        if (_type == 0)
            createshmserver();
        else
            createshmclient();
        hookshm();
    }
    char *getstart()
    {
        return _start;
    }
    ~Shm()
    {
        Leaveshm();

        if (_type == 0)
        {
            sleep(3);
            delshm();
        }
    }

private:
    void To_Hex(key_t k)
    {
        printf("0x%x\n", k);
    }

    // 1.获取唯一内存标识key
    void getkey()
    {
        _key = ftok(pathname, pro_id);
        To_Hex(_key);
        if (_key == -1)
        {
            cerr << "shm:" << errno << ":" << strerror(errno) << endl;
        }
    }

    // 2.创建共享内存
    int createshm(int flag)
    {
        int shmid = shmget(_key, gsize, flag);

        return shmid;
    }

    void createshmclient()
    {
        _shmid = createshm(IPC_CREAT);
        if (_shmid == -1)
        {
            cerr << "client shm create:" << errno << ":" << strerror(errno) << endl;
            exit(2);
        }
    }

    void createshmserver()
    {
        _shmid = createshm(IPC_CREAT | IPC_EXCL | 0666);
        if (_shmid == -1)
        {
            cerr << "server shm create:" << errno << ":" << strerror(errno) << endl;
            exit(3);
        }
    }

    // 3.挂接到进程
    void hookshm()
    {
        _start = (char *)shmat(_shmid, NULL, SHM_RND);
        if (_start == NULL)
        {
            cerr << "hookshm:" << errno << ":" << strerror(errno) << endl;
        }
    }

    // 4.进程去链接
    void Leaveshm()
    {
        int n = shmdt(_start);
        if (n == -1)
        {
            cerr << "shmdt:" << errno << ":" << strerror(errno) << endl;
        }
    }
    // 5.删除共享内存
    void delshm()
    {
        int n = shmctl(_shmid, IPC_RMID, NULL);
        if (n == -1)
        {
            cerr << "shmdt:" << errno << ":" << strerror(errno) << endl;
        }
    }

private:
    key_t _key;
    int _shmid;
    char *_start;
    int _type;
};