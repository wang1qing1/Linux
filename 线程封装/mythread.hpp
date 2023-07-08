#pragma once
// 封装一个线程，具有自己的一些自定义属性，和自己的回调函数
//
#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>

using namespace std;
struct Thread_run_ret
{
    string str;
};
class Thread
{
public:
    typedef void *(*fun_t)(void *);
    typedef enum
    {
        NEW = 0,
        RUNING,
        EXITED
    } ThreadStatus;

public:
    Thread(int num, fun_t fun, void *args)
        : _tid(0), _status(NEW), _func(fun), _args(args)
    {
        char name[128];
        snprintf(name, sizeof(name), "thread-%d", num);
        _name = name;
    }
    int status()
    {
        return _status;
    }

    string threadname()
    {
        return _name;
    }

    pthread_t threadid()
    {
        if (_status == RUNING)
        {
            return _tid;
        }
        else
        {
            return 0;
        }
    }
    void operator()()
    {
        if (_func)
        {
            _func(_args);
        }
    }
    // 由于成员函数，会有一个隐藏的参数this，占据第一个参数的位置，
    // 所以要想让pthread_create()传来的参数匹配上，就得让该线程回调函数
    // 是一个静态的成员函数。但是静态的成员函数，无法访问非静态成员
    // 所以对象指针我们需要手动传过来
    static void *runHelper(void *args)
    {
        Thread *ts = (Thread *)args;
        (*ts)();
        return nullptr;
    }
    void run()
    {
        int ret = pthread_create(&_tid, nullptr, runHelper, this);
        // this作为线程回调函数的参数，
        if (ret != 0)
        {
            exit(1);
        }
        _status = RUNING;
    }

    void join()
    {
        pthread_join(_tid, nullptr);
    }

private:
    pthread_t _tid;       // 线程id
    string _name;         // 线程name
    fun_t _func;          // 执行的回调函数
    void *_args;          // 回调函数参数
    ThreadStatus _status; // 状态
    // Thread_run_ret* _ret;
};