#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

typedef enum Status
{
    NEW = 0,
    EXIT,
    RUNNING
} Status;

class Thread
{
private:
    typedef void *(*Func)(void *);

public:
    Thread(int num, Func func, void *args)
        : _func(func), _args(args)
    {
        char __name[50] = {0};
        sprintf(__name, "thread-%d", num);
        _name = __name;
        _status = NEW;
    }

    static void *RunFunc(void *args)
    {
        Thread *ts = static_cast<Thread *>(args);
        return (*ts)();
    }

    void *operator()()
    {
        return _func(_args);
    }

    void run()
    {
        pthread_create(&_tid, NULL, RunFunc, this);
        _status = RUNNING;
    }

    void *join()
    {
        void **ret = (void **)new void *;
        pthread_join(_tid, ret);
        _status = EXIT;
        return *ret;
    }

    string name()
    {
        return _name;
    }

    ~Thread()
    {
    }

private:
    Func _func;
    string _name;
    pthread_t _tid;
    Status _status;
    void *_args;
};