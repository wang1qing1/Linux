#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include <unistd.h>
#include "Task.hpp"
#include "../../23_10_11/Class_mymutex/mutex.hpp"

using namespace std;
const size_t gsize = 5;
template <class T>
class Thread_Pool
{
public:
    Thread_Pool(size_t num = gsize)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        _threads.resize(num);
    }

    void push_task(const T task)
    {
        MutexGuard lock(&_mutex);
        _task_q.push(task);
        condsignal();
    }

    void Init()
    {
    }

    void Start()
    {
        for (int i = 0; i < _threads.size(); i++)
        {
            pthread_create(&_threads[i], nullptr, thread_func, this);
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
    vector<pthread_t> _threads;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    queue<T> _task_q;
};