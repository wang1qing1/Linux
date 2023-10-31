#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <functional>

using namespace std;

typedef enum Status
{
    NEW = 0,
    EXIT,
    RUNNING
} Status;

class Thread
{
public:
    // 构造函数
    Thread(int num, function<void *(void *)> func, void *args)
        : _func(func), _args(args)
    {

        char __name[50] = {0};
        sprintf(__name, "thread-%d", num);
        _name = __name;
        _status = NEW;
    }

    // 线程开始执行
    void run()
    {
        pthread_create(&_tid, NULL, RunFunc, this);
        _status = RUNNING;
    }

    // 线程等待
    void *join()
    {
        void **ret = (void **)new void *;
        pthread_join(_tid, ret);
        _status = EXIT;
        return *ret;
    }

    // 返回线程的名字
    string name()
    {
        return _name;
    }

    ~Thread()
    {
    }

private:
    // 如果类的回调函数是一个类的成员函数，那么这个成员函数需要是一个
    // 静态成员函数，因为普通成员函数有this指针的干扰
    static void *RunFunc(void *args)
    {
        Thread *ts = static_cast<Thread *>(args);
        return (*ts)();
    }

    void *operator()()
    {
        return _func(_args);
    }

private:
    function<void *(void *)> _func; // 回调的函数
    string _name;                   // 线程名称
    pthread_t _tid;                 // 线程id
    Status _status;                 // 线程状态
    void *_args;                    // 线程回调函数的参数
};