#pragma once

class Mutex
{

public:
    // // 动态时
    // Mutex(pthread_mutex_t *mutex)
    //     : _mutex(mutex)
    // {
    // error/重复初始化
    //    pthread_mutex_init(_mutex, NULL);
    // }

    // 静态时：
    Mutex(pthread_mutex_t *mutex)
        : _mutex(mutex)
    {
    }

    void lock()
    {
        pthread_mutex_lock(_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_mutex);
    }
    ~Mutex()
    {
    }

private:
    pthread_mutex_t *_mutex;
};

class MutexGuard
{
public:
    MutexGuard(pthread_mutex_t *mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }
    ~MutexGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex _mutex;
};
