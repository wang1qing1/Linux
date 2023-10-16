#pragma once

#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <semaphore.h>
#include "../../23_10_11/Class_mymutex/mutex.hpp"
#include "Task.hpp"

using namespace std;

const size_t size = 5;

template <class T>
class RingQueue
{

    void P(sem_t &sem) // 申请信号量
    {
        sem_wait(&sem);
    }

    void V(sem_t &sem) // 释放信号量
    {
        sem_post(&sem);
    }

public:
    RingQueue(int cap = size)
        : _cap(cap), _index_space(0), _index_date(0)
    {
        sem_init(&_sem_date, 0, 0);
        sem_init(&_sem_space, 0, cap);
        pthread_mutex_init(&_mutex, nullptr);
        _rq.resize(_cap);
    }

    void push(const T date)
    {
        P(_sem_space);
        {
            MutexGuard lock(&_mutex);
            _rq[_index_date++] = date;

            _index_date %= _cap;
        }
        V(_sem_date);
    }

    T pop()
    {
        P(_sem_date);
        T tmp;
        {
            MutexGuard lock(&_mutex);
            tmp = _rq[_index_space++];
            _index_space %= _cap;
        }
        V(_sem_space);
        return tmp;
    }

    ~RingQueue()
    {
        sem_destroy(&_sem_date);
        sem_destroy(&_sem_space);
        pthread_mutex_destroy(&_mutex);
    }

private:
    vector<T> _rq;
    size_t _cap;
    sem_t _sem_space;
    sem_t _sem_date;

    size_t _index_space;
    size_t _index_date;

    pthread_mutex_t _mutex;
};