#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include <unistd.h>
#include "Task.hpp"
#include "../../23_10_11/Class_mythread/mythread.hpp"
#include "../../23_10_11/Class_mymutex/mutex.hpp"

using namespace std;
const size_t gsize = 3;

template <class T>
class Thread_Pool
{
public:
    Thread_Pool()
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }

    void push_task(const T task)
    {
        MutexGuard lock(&_mutex);
        _task_q.push(task);
        condsignal();
    }

    void Init()
    {
        for (int i = 0; i < gsize; i++)
        {
            _threads.push_back(Thread(i, thread_func, this));
        }
    }

    void Start()
    {
        for (auto &thread : _threads)
        {
            thread.run();
        }
    }

    ~Thread_Pool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
        _threads.clear();
    }

private:
    static void *thread_func(void *args)
    {
        pthread_detach(pthread_self());
        Thread_Pool<T> *tp = static_cast<Thread_Pool<T> *>(args);
        while (1)
        {
            Task task;
            {
                MutexGuard lock(&tp->_mutex);
                while (tp->isempty())
                {
                    // cout << "11111111" << endl;

                    tp->condwait();
                }
                task = tp->_task_q.front();
                tp->_task_q.pop();
            }
            task.run();
            task.showresult();
        }
    }
    bool isempty()
    {
        return _task_q.empty();
    }

    void condwait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void condsignal()
    {
        pthread_cond_signal(&_cond);
    }

private:
    vector<Thread> _threads;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
    queue<T> _task_q;
};